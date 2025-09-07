//
// Created by Joe (Personal) on 10/02/2025.
//

#include <amqp.h>
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/http.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "amqp.h"
#include <jansson.h>
#include "config.h"
#include "queue.h"

#define FMQ_AMQP_PORT 5672

static struct evconnlistener *listener = NULL;

void send_connection_start(struct bufferevent *bev)
{
    // connection.start payload
    unsigned char payload[] =
    {
        0x00, 0x0A,         // Class ID = 10 (connection)
        0x00, 0x0A,         // Method ID = 10 (start)
        0x00,               // version-major
        0x09,               // version-minor
        0x00, 0x00,         // server properties (empty table, mocked)
        0x00,               // mechanisms length = 0 (mocked)
        0x00                // locals length = 0 (mocked)
    };

    size_t payload_size = sizeof(payload);
    unsigned char frame[1024] = {0};


}


static void amqp_read_callback(struct bufferevent *bev, void *ctx)
{
    FMQ_Queue *queue = (FMQ_Queue*)ctx;
    struct evbuffer *input = bufferevent_get_input(bev);
    size_t len = evbuffer_get_length(input);

    // Check that the header matches the AMQP protocol of 8 bytes
    if (len < 8)
    {
        FMQ_LOGGER(queue->log_level, "AMQP incoming header less that 8 bytes");
        return;
    }

    unsigned char header[8];
    evbuffer_remove(input, header, 8);

    // "AMQP\0\0\x09\x01"
    const unsigned char expected[8] = {
        'A', 'M', 'Q', 'P', 0, 0, 9, 1
    };

    if (memcmp(header, expected, 8) ==0)
    {
        FMQ_LOGGER(queue->log_level, "AMQP Protocol received & validated successfully\n");
        // TODO
    }
    else
    {
        FMQ_LOGGER(queue->log_level, "AMQP Invalid protocol header\n");
        bufferevent_free(bev);
    }
}
static void amqp_event_callback(struct bufferevent *bev, short events, void *ctx)
{
    FMQ_Queue *queue = (FMQ_Queue*)ctx;
    if (events & BEV_EVENT_EOF)
    {
        FMQ_LOGGER(queue->log_level, "AMQP Client connected\n");
    }
    else if (events & BEV_EVENT_ERROR)
    {
       perror("AMQP Connection error");
    }
    bufferevent_free(bev);
}

static void amqp_accept_callback(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *address, int socklen, void *ctx)
{
    FMQ_Queue *queue = (FMQ_Queue *)ctx;

    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    if (!bev)
    {
        perror("[AMQP] Failed to create a bufferevent");
        close(fd);
        return;
    }

    // Read & event handlers
    bufferevent_setcb(bev, amqp_read_callback, NULL, amqp_event_callback, queue);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    FMQ_LOGGER(queue->log_level, "AMPQ Client connected on port %d\n", FMQ_AMQP_PORT);
}

void start_amqp_listener(struct event_base *base, FMQ_Queue *queue)
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    sin.sin_port = htons(FMQ_AMQP_PORT);

    listener = evconnlistener_new_bind(
        base,
        amqp_accept_callback,
        queue,
       LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
       -1,
       (struct sockaddr *)&sin,
       sizeof(sin)
    );

    if (!listener)
    {
        perror("AMPQ Failed to bind to port 5672");
        exit(1);
    }
    FMQ_LOGGER(queue->log_level, "Listening for AMQP connections on port %d\n", FMQ_AMQP_PORT);
}
