#ifndef __COMMON__
#define __COMMON__

#include <stdlib.h>
#include <fcntl.h>

#define LOG_R(x, ...) fprintf(stderr, "\033[31m" x "\033[0m" __VA_OPT__(,) __VA_ARGS__)
#define LOG_G(x, ...) fprintf(stderr, "\033[32m" x "\033[0m" __VA_OPT__(,) __VA_ARGS__)
#define LOG_Y(x, ...) fprintf(stderr, "\033[33m" x "\033[0m" __VA_OPT__(,) __VA_ARGS__)

#define CALLOC_SELF(x)                      \
if (!(x = calloc(1, sizeof(*x)))) {         \
    LOG_R("ERROR: calloc " #x "error\n");   \
    return NULL;                            \
}
#define SET_NONBLOCK(fd)                    \
{                                           \
    int flags = fcntl(fd, F_GETFL, 0);      \
    fcntl(fd, F_SETFL, flags |  O_NONBLOCK);\
}
#define SET_BLOCK(fd)                       \
{                                           \
    int flags = fcntl(fd, F_GETFL, 0);      \
    fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);\
}

#endif//__COMMON__
