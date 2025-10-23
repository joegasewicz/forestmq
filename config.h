//
// Created by Joe Gasewicz on 15/06/2024.
//

#ifndef CONFIG_H
#define CONFIG_H
#include <stdarg.h>
#include <stdio.h>
/* ANSI Colors */
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_RESET    "\x1b[0m"
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

#define _FMQ_LOGGER(level, color, ...) \
{ \
    if (level > 0) \
    { \
        printf(color "[FORESTMQ]: "  __VA_ARGS__);  \
        printf(ANSI_COLOR_RESET); \
        fflush(stdout); \
    } \
}\

#define FMQ_LOGGER_WHITE(level, ...) FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define FMQ_LOGGER_RED(level, ...) _FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, ANSI_COLOR_RED, __VA_ARGS__)
#define FMQ_LOGGER_GREEN(level, ...) _FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, ANSI_COLOR_GREEN, __VA_ARGS__)
#define FMQ_LOGGER_YELLOW(level, ...) _FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, ANSI_COLOR_YELLOW, __VA_ARGS__)
#define FMQ_LOGGER_BLUE(level, ...) _FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, ANSI_COLOR_BLUE, __VA_ARGS__)
#define FMQ_LOGGER_MAGENTA(level, ...) _FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, ANSI_COLOR_MAGENTA, __VA_ARGS__)
#define FMQ_LOGGER_CYAN(level, ...) _FMQ_LOGGER(FMQ_LOG_LEVEL_DEBUG, ANSI_COLOR_CYAN, __VA_ARGS__)

/* Macros */
#define FMQ_ARRAY_LENGTH(arr) sizeof(arr) / sizeof(arr[0])
/* TCP configs */
#define FMQ_TCP_PORT 8005
#define FMQ_MESSAGE_SIZE 1024 * 5  // Python requests JSON body is always > 10000
#define FMQ_ALLOWED_HOSTS_LENGTH 10
#define FMQ_ALLOWED_HOSTS_BYTES 1080
#define FMQ_DEFAULT_ALLOWED_HOSTS "localhost,0.0.0.0,127.0.0.1,host.docker.internal"
/* ANSI STRING */
#define FMQ_PREAMBLE_MSG "\n" \
"   / ____/ __ \\/ __ \\/ ____/ ___/_  __/  /  |/  / __ \\\n" \
"  / /_  / / / / /_/ / __/  \\__ \\ / /    / /|_/ / / / /\n" \
" / __/ / /_/ / _, _/ /___ ___/ // /    / /  / / /_/ / \n" \
"/_/    \\____/_/ |_/_____//____//_/    /_/  /_/\\___\\_\\ \n\n"


#endif //CONFIG_H
