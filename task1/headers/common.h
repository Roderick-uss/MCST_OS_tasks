#ifndef __COMMON__
#define __COMMON__

#define LOG_R(x, ...) fprintf(stderr, "\033[31m" x "\033[0m" __VA_OPT__(,) __VA_ARGS__)
#define LOG_G(x, ...) fprintf(stderr, "\033[32m" x "\033[0m" __VA_OPT__(,) __VA_ARGS__)
#define LOG_Y(x, ...) fprintf(stderr, "\033[33m" x "\033[0m" __VA_OPT__(,) __VA_ARGS__)

#endif//__COMMON__
