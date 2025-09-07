//
// Created by Joe Gasewicz on 07/10/2024.
//

#ifndef SERVER_H
#define SERVER_H
#include <stdint.h>
#include <stdbool.h>
#include "queue.h"


typedef struct FMQ_Server           FMQ_Server;

struct FMQ_Server
{
    int                 (*start)(FMQ_Server *server);
    FMQ_Queue           *queue;
    uint16_t            port;
    int8_t              log_level;
    bool                run_as_daemon;
    char                **allowed_hosts;
    int                 allowed_hosts_len;
};


FMQ_Server *FMQ_Server_new(FMQ_Queue *queue, const uint16_t port, int8_t log_level,
    bool run_as_daemon, char *hosts);

#endif //SERVER_H
