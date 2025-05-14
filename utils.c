//
// Created by Joe Gasewicz on 09/10/2024.
//
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <jansson.h>
#include "server.h"
#include "utils.h"

/**
 * Used to check if the current incoming request's host matches the allowed hosts array
 * @param request_host 
 * @param allowed_hosts
 * @param allowed_hosts_len
 * @return returns true if a single host matches the incoming requests host
 */
bool check_allowed_hosts(const char *request_host, char **allowed_hosts, int allowed_hosts_len)
{
    for (int i = 0; i < allowed_hosts_len; i++)
    {
        if (strcmp(request_host, allowed_hosts[i]) == 0)
        {
            return true;
        }
    }
    return false;
}


/**
 * Gets the request's body as a pointer to a string & stores in body_data
 * @param json_body The caller is required to free body_data
 * @param req libevents requests
 * @param q pointer to FMQ_Queue
 */
void get_request_body(char *json_body, struct evhttp_request *req, FMQ_Queue *q)
{
    // network operation
    struct evbuffer *input_buffer = evhttp_request_get_input_buffer(req);
    size_t body_len = evbuffer_get_length(input_buffer);
    // allocate memory to store the request body
    char *body_data = malloc(body_len + 1);
    if (body_data)
    {
        evbuffer_copyout(input_buffer, body_data, body_len);
        body_data[body_len] = '\0';
        memcpy(json_body, body_data, body_len);
        free(body_data);
    }
}