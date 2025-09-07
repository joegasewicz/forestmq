//
// Created by Joe Gasewicz on 07/10/2024.
//

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <jansson.h>
#include "config.h"
#include "utils.h"
#include "tcp.h"
#include "server.h"
#include "amqp.h"


static char* get_req_method(enum evhttp_cmd_type cmd)
{
    switch (cmd) {
        case EVHTTP_REQ_GET: return "GET";
        case EVHTTP_REQ_POST: return "POST";
        case EVHTTP_REQ_HEAD: return "HEAD";
        case EVHTTP_REQ_PUT: return "PUT";
        case EVHTTP_REQ_DELETE: return "DELETE";
        case EVHTTP_REQ_OPTIONS: return "OPTIONS";
        case EVHTTP_REQ_TRACE: return "TRACE";
        case EVHTTP_REQ_CONNECT: return "CONNECT";
        case EVHTTP_REQ_PATCH: return "PATCH";
        default: return "unknown";
    }
}

static void resp_callback(struct evhttp_request *req, void *server)
{
    const FMQ_Server *s = (FMQ_Server*)server;
    const FMQ_Queue *q = (FMQ_Queue*)s->queue;
    struct evbuffer *reply = evbuffer_new();     // <-- Allocate storage for a new evbuffer.

    const char *request_uri = evhttp_request_get_uri(req);
    const char *request_host = evhttp_request_get_host(req);
    const bool host_allowed = check_allowed_hosts(request_host, s->allowed_hosts, s->allowed_hosts_len);

    struct evhttp_uri *parsed_uri = evhttp_uri_parse(request_uri);
    if (!parsed_uri)
    {
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    enum evhttp_cmd_type cmd = evhttp_request_get_command(req);
    const char *method = get_req_method(cmd);
    if (strcmp(method, "unknown") == 0)
    {
        FMQ_LOGGER(q->log_level, "Host:%s not allowed\n", request_host);
        evhttp_send_reply(req, 400, "Method not recognized", reply);
    }
    else if (host_allowed == false)
    {
        evhttp_send_reply(req, 403, "Host not allowed", reply);
    }
    else
    {
        FMQ_LOGGER(q->log_level, "{server}: %s %s HTTP/1.1 \n",  method, request_uri);

        // check the method & path
        if (strcmp(method, "POST") == 0 && strcmp(request_uri, "/provider") == 0)
        {
            provider_callback(req, reply, s->queue);
        }
        else if (strcmp(method, "POST") == 0 && strcmp(request_uri, "/consumer") == 0)
        {
            consumer_callback(req, reply, s->queue);
        }
        else if (strcmp(method, "GET") == 0 && strcmp(request_uri, "/health") == 0)
        {
            health_callback(req, reply, s->queue);
        }
        else
        {
            evbuffer_add_printf(reply, "NULL"); // TODO 404
        }

        struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
        evhttp_add_header(headers, "Content-Type", "application/json");
        evhttp_send_reply(req, HTTP_OK, NULL, reply);
    }
    // cleanup
    if (parsed_uri)
    {
        evhttp_uri_free(parsed_uri);
    }
    evbuffer_free(reply);
}

static void my_signal_event_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event_base *base = (struct event_base *) arg;
    event_base_loopbreak(base);
}


static int start_server(FMQ_Server *s)
{
    char http_addr[] = "0.0.0.0";
    ev_uint16_t http_port = (ev_uint16_t)s->port;

    struct event_base *base = event_base_new();

    // TODO Start AMPQ listener on port 5672 using the same event base
    // start_amqp_listener(base, s->queue); TODO

    struct evhttp *http_server = evhttp_new(base);

    int http_handle = evhttp_bind_socket(http_server, (const char *) http_addr, http_port);

    // evhttp_set_max_body_size() TODO
    // handlers
    evhttp_set_gencb(http_server, resp_callback, s);
    // Add SIGINT event

    struct event *sig_int = evsignal_new(base, SIGINT, my_signal_event_cb, base);
    event_add(sig_int, NULL);

    FMQ_LOGGER(s->log_level, "Listening on http://%s:%d (%d)\n", http_addr, http_port, http_handle);
    event_base_dispatch(base);

    evhttp_free(http_server);
    event_free(sig_int);
    event_base_free(base);
    return 0;
}

FMQ_Server *FMQ_Server_new(FMQ_Queue *queue, const uint16_t port,
                       const int8_t log_level, bool run_as_daemon, char *hosts)
{
    FMQ_Server *server = (FMQ_Server*)malloc(sizeof(FMQ_Server));
    server->queue = queue;
    server->start = start_server;
    server->port = port;
    server->log_level = log_level;
    server->run_as_daemon = run_as_daemon;
    // allowed hosts
    int maxTokens = 0; // get the number of hosts
    for (int i = 0; hosts[i] != '\0'; i++)
    {
        if (hosts[i] == ',')
            maxTokens++;
    }
    maxTokens++; // 1+ for the last host
    server->allowed_hosts_len = maxTokens;
    server->allowed_hosts = malloc(maxTokens * sizeof(char *));
    if (server->allowed_hosts == NULL)
    {
        perror("server->allowed_hosts malloc fail");
        exit(1);
    }
    int index = 0;
    char *token;
    char * delimiter = ",";
    token = strtok(hosts, delimiter);
    while (token != NULL)
    {
        server->allowed_hosts[index] = strdup(token);
        if (server->allowed_hosts[index] == NULL)
        {
            perror("strdup failed for server->allowed_hosts");
            exit(1);
        }
        index++;
        token = strtok(NULL, delimiter);
    }
    return server;
}