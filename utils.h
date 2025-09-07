//
// Created by Joe Gasewicz on 09/10/2024.
//

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "queue.h"


bool check_allowed_hosts(const char *request_host, char **allowed_hosts,
    int allowed_hosts_len);

void get_request_body(char *json_body, struct evhttp_request *req, FMQ_Queue *q);

#endif //UTILS_H
