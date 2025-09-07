//
// Created by Joe Gasewicz on 15/06/2024.
//

#ifndef CONFIG_H
#define CONFIG_H
#include <stdarg.h>
#include <stdio.h>

/* Logging types */
#define FMQ_LOG_LEVEL_NONE 0
#define FMQ_LOG_LEVEL_DEBUG 1

#define FMQ_LOGGER(level, ...) \
{ \
if (level > 0) \
{ \
printf("[FORESTMQ]: " __VA_ARGS__);  \
fflush(stdout); \
} \
}\

/* Macros */
#define FMQ_ARRAY_LENGTH(arr) sizeof(arr) / sizeof(arr[0])

/* TCP configs */
#define FMQ_TCP_PORT 8005
#define FMQ_MESSAGE_SIZE 1024 * 5  // Python requests JSON body is always > 10000

#define FMQ_ALLOWED_HOSTS_LENGTH 10
#define FMQ_ALLOWED_HOSTS_BYTES 1080
#define FMQ_DEFAULT_ALLOWED_HOSTS "localhost,0.0.0.0,127.0.0.1,host.docker.internal"

#endif //CONFIG_H
