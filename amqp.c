//
// Created by Joe (Personal) on 10/02/2025.
//


#include <amqp.h>
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "amqp.h"

#define AMQP_PORT 5672

struct event_base *ev_base;
struct evconnlistener *amqp_listener;

void amqp_accept_callback(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *addr, int socklen, void *ctx)
{
    printf("[AMQP] New client connected\n");
    // TODO Create an AMPQ conn object & manage session state
}

void setup_amqp_listener(int port)
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htonl(INADDR_ANY);
    sin.sin_port = htons(port);

    ev_base = event_base_new();
    if (!ev_base)
    {
        fprintf(stderr, "Error: Could not create event base\n");
        exit(1);
    }

    amqp_listener = evconnlistener_new_bind(
        ev_base, amqp_accept_callback, NULL,
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin, sizeof(sin)
    );

    if (!amqp_listener)
    {
        fprintf(stderr, "Error: Could not create AMQP listener\n");
        exit(1);
    }

    printf("[AMQP] Listening for AMQP connections on port %d...\n", port);
    event_base_dispatch(ev_base);
}