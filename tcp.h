//
// Created by Joe (Personal) on 10/02/2025.
//

#ifndef TCP_H
#define TCP_H

void consumer_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue);
void provider_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue);
void health_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue);

#endif //TCP_H
