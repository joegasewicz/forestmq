cmake_minimum_required(VERSION 3.22)
project(forest_mq C)

set(CMAKE_C_STANDARD 11)

add_executable(forest_mq main.c
        consumer.c
        consumer.h
        provider.c
        provider.h
        queue.c
        queue.h
        config.c
        config.h
        server.c
        server.h
        utils.c
        utils.h
        amqp.c
        amqp.h
        tcp.c
        tcp.h
)

find_package(PkgConfig REQUIRED)


if (APPLE)
    EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    message( STATUS "Mac arch: ${ARCHITECTURE}" )
    if (ARCHITECTURE MATCHES "x86_64")
        set(BREW_ROOT "/usr/local")
        message("Setting brew root as ${BREW_ROOT} for ${ARCHITECTURE}")

    else ()
        set(BREW_ROOT "/opt/homebrew")
        message("Setting brew root as ${BREW_ROOT} for ${ARCHITECTURE}")
    endif ()
    target_include_directories(
            forest_mq
            PRIVATE
            ${BREW_ROOT}/Cellar/jansson/2.14.1/include
            ${BREW_ROOT}/Cellar/libevent/2.1.12_1/include
            ${BREW_ROOT}/Cellar/rabbitmq-c/0.15.0/include
    )
elseif (UNIX)
    target_include_directories(
            forest_mq
            PRIVATE
            /usr/lib/x86_64-linux-gnu
            /usr/lib/x86_64-linux-gnu/libjansson.so
    )
endif()

if (APPLE)
    message("Setting target_link_libraries brew root as ${BREW_ROOT} for ${ARCHITECTURE}")
    target_link_libraries(
            forest_mq
            PRIVATE
            ${BREW_ROOT}/Cellar/jansson/2.14.1/lib/libjansson.dylib
            ${BREW_ROOT}/Cellar/libevent/2.1.12_1/lib/libevent.dylib
            ${BREW_ROOT}/Cellar/rabbitmq-c/0.15.0/lib/librabbitmq.dylib
    )
elseif(UNIX)
    target_link_libraries(
            forest_mq
            PRIVATE
            jansson
            event
            rabbitmq
    )
endif ()