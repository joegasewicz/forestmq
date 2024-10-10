/**
* MIT License
*
* Copyright (c) 2024 Joe Gasewicz
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include "config.h"
#include "queue.h"
#include "server.h"

int main(int argc, char *argv[])
{
    u_int16_t msg_size = FMQ_MESSAGE_SIZE;
    u_int16_t port = FMQ_TCP_PORT;
    int8_t log_level = FMQ_LOG_LEVEL_NONE;
    char allowed_hosts[FMQ_ALLOWED_HOSTS_BYTES] = FMQ_DEFAULT_ALLOWED_HOSTS;
    bool run_as_daemon = false;
    pid_t daemon_pid;
    const char *FORESTMQ_DAEMON = getenv("FORESTMQ_DAEMON");
    const char *FORESTMQ_PORT = getenv("FORESTMQ_PORT");

    if (FORESTMQ_DAEMON && strcmp(FORESTMQ_DAEMON, "1") == 0)
    {
        run_as_daemon = true;
        FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, "Running ForestMQ in daemon mode\n");
    }
    if (FORESTMQ_PORT)
    {
        port = atoi(FORESTMQ_PORT);
        if (port < 80)
            goto port_arg_error;
        FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, "Set TCP Server port to %s\n", FORESTMQ_PORT);
    }
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--msg-size") == 0)
        {
            if (argc < i+1)
            {
                printf("--msg-size arg expects an integer value\n");
                exit(EXIT_FAILURE);
            }
            char *msg_size_char = argv[i+1];
            msg_size = atoi(msg_size_char);
            if (msg_size <= 0)
                goto msg_size_arg_error;
            FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, "Set queue message size to %s\n", msg_size_char);
        }
        if (strcmp(argv[i], "--port") == 0)
        {
            if (argc < i+1)
                goto port_arg_error;
            char *port_char = argv[i+1];
            port = atoi(port_char);
            if (port < 80)
                goto port_arg_error;
            FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, "Set TCP Server port to %s\n", port_char);
        }
        if (strcmp(argv[i], "--log-level") == 0)
        {
            if (argc < i+1)
                goto log_arg_error;
            char *log_level_char = argv[i+1];
            if (log_level_char == NULL)
                goto log_arg_error;
            if (strcmp(log_level_char, "none") == 0)
                log_level = FMQ_LOG_LEVEL_NONE;
            else if (strcmp(log_level_char, "debug") == 0)
                log_level = FMQ_LOG_LEVEL_DEBUG;
            else goto log_arg_error;
            FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, "Log level set to %s\n", log_level_char);
        }
        if (strcmp(argv[i], "--daemon") == 0)
        {
            if (!run_as_daemon)
            {
                run_as_daemon = true;
                FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, "Running ForestMQ in daemon mode\n");
            }
        }
    }

    FMQ_Queue *queue = FMQ_Queue_new(msg_size, log_level);
    FMQ_Server *server = FMQ_Server_new(queue, port, log_level, run_as_daemon);
    if (server->run_as_daemon)
    {
         daemon_pid = fork();
        if (daemon_pid == -1)
        {
            fprintf(stderr, "Error forking daemon process\n");
            exit(EXIT_FAILURE);
        }
        if (daemon_pid == 0)
        {
            const int err = server->start(server);
            if (server != NULL)
            {
                free(server);
            }
            free(queue);
            if (err)
            {
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        const int err = server->start(server);
        if (server != NULL)
        {
            free(server);
        }
        free(queue);
        if (err)
        {
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;

msg_size_arg_error:
    printf("--msg-size arg expects an unsigned integer value\n");
    exit(EXIT_FAILURE);
port_arg_error:
    printf(
        "Arg Error:\n"
        "--port arg must be set to 80 or above\n"
    );
    exit(EXIT_FAILURE);

log_arg_error:
    printf(
        "Arg Error:\n"
        "--log-level should be set to the following:\n"
        "\t`--log-level none` - no logs\n"
        "\t`--log-level debug` - all logs\n"
    );
    exit(EXIT_FAILURE);
}
