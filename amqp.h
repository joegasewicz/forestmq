//
// Created by Joe (Personal) on 10/02/2025.
//

#ifndef AMQP_H
#define AMQP_H

#include "queue.h"

void start_amqp_listener(struct event_base *base, FMQ_Queue *queue);


#endif //AMQP_H
