//
// Created by Joe (Personal) on 10/02/2025.
//
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <jansson.h>
#include "server.h"
#include "config.h"
#include "utils.h"
#include "tcp.h"


void consumer_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue)
{
    const FMQ_Queue *q = (FMQ_Queue*)queue;
    const FMQ_QNode *node = FMQ_Queue_dequeue((FMQ_Queue*)queue);
    if (node == NULL)
    {
        FMQ_LOGGER(q->log_level ,"{consumer}: Queue is empty\n");
        json_t *root = json_object();
        json_object_set_new(root, "error", json_string("Queue is empty!"));
        char *json_str = json_dumps(root, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
        free(json_str);
        json_decref(root);
        return;
    }
    const FMQ_Data *dataPtr = (FMQ_Data*)node->data;
    json_t *message_load = json_loads(dataPtr->message, JSON_ENCODE_ANY, NULL);
    if (json_is_null(message_load))
    {
        const char *err_msg = "{consumer}: Error: No message in stored queue node.";
        json_t *err_json = json_object();
        json_object_set_new(err_json, "error", json_string(err_msg));
        FMQ_LOGGER(q->log_level, "{consumer}: Error: No message in stored queue node.\n");
        char *json_str = json_dumps(err_json, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
        free(json_str);
        json_decref(err_json);
        return;
    }
    char *msg_dump = json_dumps(message_load, JSON_COMPACT);
    FMQ_LOGGER(q->log_level, "{consumer}: Successfully dequeued message for consumer\n");
    FMQ_LOGGER(q->log_level, "{consumer}: Received: %s\n", msg_dump);
    free(msg_dump);
    free((FMQ_Data*)node->data);
    free((FMQ_QNode*)node);
    json_t *json_res_object = json_object();
    JSON_INDENT(4);
    json_object_set_new(json_res_object, "status", json_string(q->status));
    json_object_set_new(json_res_object, "queue_length", json_integer(q->size));
    json_object_set_new(json_res_object, "message_size", json_integer(q->msg_size));
    json_object_set_new(json_res_object, "message", message_load);
    char *json_str = json_dumps(json_res_object, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
    free(json_str);
    json_decref(message_load);
    json_decref(json_res_object);
}


void provider_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue)
{
    FMQ_Queue *q = (FMQ_Queue*)queue;
    char body_data[q->msg_size]; // stack buffer
    memset(body_data, 0, sizeof(body_data));

    get_request_body(body_data, req, q);

    json_error_t error;
    json_t *json_req_object = json_loads(body_data, JSON_INDENT(0), &error);
    if (strlen(error.text) > 0)
        FMQ_LOGGER(q->log_level, "{provider} ERROR: %s\n", error.text);
    // get JSON object's values
    json_t *message = json_object_get(json_req_object, "message");
    if (message == NULL)
    {
        FMQ_LOGGER(q->log_level, "{provider} ERROR: No JSON in request body\n");
        json_t *root = json_object();
        json_object_set_new(root, "error", json_string("expected 'message' key in JSON"));
        char *json_str = json_dumps(root, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
        free(json_str);
        json_decref(root);
        json_decref(json_req_object);
        return;
    }
    const bool destroy = json_boolean_value(json_object_get(json_req_object, "destroy"));
    if (destroy)
    {
        FMQ_LOGGER(q->log_level, "{provider}: Successfully destroyed queue\n");
        char *json_str = json_dumps(message, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
        free(json_str);
        json_decref(message);
        json_decref(json_req_object);
        return;
    }
    // Handle successful request & do queue operations
    FMQ_LOGGER(q->log_level, "{provider}: Received message successfully\n");
    FMQ_Data *data = (FMQ_Data*)malloc(sizeof(FMQ_Queue));
    data->message = malloc(sizeof(char) * q->msg_size);
    // json_dumps must be freed before we can deallocate message
    char *json_q_dump = json_dumps(message, JSON_COMPACT);
    strcpy(data->message, json_q_dump);
    FMQ_Queue_enqueue(q, data);

    // Prepare response
    json_t *json_res_object = json_object();
    json_t *message_body = json_pack("o*", message);
    json_object_set_new(json_res_object, "status", json_string(q->status));
    json_object_set_new(json_res_object, "queue_length", json_integer(q->size));
    json_object_set_new(json_res_object, "message_size", json_integer(q->msg_size));
    json_object_set_new(json_res_object, "message", message_body);
    char *json_res_str = json_dumps(json_res_object, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
    evbuffer_add_printf(reply,  json_res_str);
#pragma GCC diagnostic pop
    free(json_q_dump);
    free(json_res_str);
    json_decref(message_body);
    json_decref(json_req_object);
    json_decref(json_res_object);
    json_decref(message);
}


void health_callback(struct evhttp_request *req, struct evbuffer *reply, void *queue)
{
    time_t rawtime_start, rawtime_end;
    struct tm *req_start;
    struct tm *req_end;

    time(&rawtime_start);
    req_start = localtime(&rawtime_start);

    json_t *root = json_object();
    bool queue_is_empty = false;
    JSON_INDENT(4);
    const FMQ_Queue *q = (FMQ_Queue*)queue;
    const FMQ_QNode *node = FMQ_QUEUE_PEEK((FMQ_Queue*)queue);
    const int queue_len = FMQ_QUEUE_SIZE((FMQ_Queue*)queue);

    if (node == NULL)
        queue_is_empty = true;

    json_object_set_new(root, "queue_empty", json_boolean(queue_is_empty));
    json_object_set_new(root, "queue_length", json_integer(queue_len));
    json_object_set_new(root, "status", json_string("OK"));
    json_object_set_new(root, "request_start", json_string(asctime(req_start)));
    time(&rawtime_end);
    req_end = localtime(&rawtime_end);
    json_object_set_new(root, "request_end", json_string(asctime(req_end)));
    char *json_str = json_dumps(root, JSON_INDENT(4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
    evbuffer_add_printf(reply,  json_str);
#pragma GCC diagnostic pop
    free(json_str);
    json_decref(root);
}