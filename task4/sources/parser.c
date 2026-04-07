#include "parser.h"
#include "common.h"

const char short_option_list[] = "mcst";
const char * const long_option_list[] = {"elbrus=1c+", "elbrus=2c+", "elbrus=2c3", "elbrus=4c", "elbrus=8c", "elbrus=16c"};

static int handle_short (const char* arg, int* mask) {
    if (arg == NULL) {
        LOG_R("arg is NULL");
        return -1;
    }
    if (mask == NULL) {
        LOG_R("short options mask is NULL");
        return -1;
    }

    for (size_t i = 0; arg[i] != '\0'; ++i) {
        int exist_flag = 0;

        for (size_t opt_i = 0; short_option_list[opt_i] != '\0'; ++opt_i) {
            if (short_option_list[opt_i] == arg[i]) {
                *mask |= 1 << opt_i;
                exist_flag = 1;
                break;
            }
        }

        if (!exist_flag) {
            printf("Incorrect option: '%c'\n", arg[i]);
            return -1;
        }
    }

    if (arg[0] == '\0') {
        printf("Incorrect option: ''\n");
        return -1;
    }

    return 0;
}
static int handle_long  (const char* arg, int* mask ) {
    if (arg == NULL) {
        LOG_R("arg is NULL");
        return -1;
    }
    if (mask == NULL) {
        LOG_R("long options mask is NULL");
        return -1;
    }

    size_t long_option_list_size = sizeof(long_option_list) / sizeof(*long_option_list);

    for (size_t opt_i = 0; opt_i < long_option_list_size; ++opt_i) {
        if (strcmp(long_option_list[opt_i], arg) == 0) {
            *mask |= 1 << opt_i;
            return 0;
        }
    }

    printf("Incorrect option: '%s'\n", arg);

    return -1;
}
static int handle_non   (const char* arg, const char** non_option_list) {
    if (arg == NULL) {
        LOG_R("arg is NULL");
        return -1;
    }
    if (non_option_list == NULL) {
        LOG_R("non options list mask is NULL");
        return -1;
    }

    const char** current_opt = non_option_list;
    while (*current_opt != NULL) {
        if (strcmp(arg, *current_opt) == 0) return 0;
        current_opt++;
    }

    *current_opt = arg;
    return 0;
}

static void print_short (int mask) {
    printf("Short options:");
    for (size_t i = 0; short_option_list[i] != '\0'; ++i) {
        if (mask & (1<<i)) printf(" '%c'", short_option_list[i]);
    }
    putc('\n', stdout);
}
static void print_long  (int mask) {
    size_t long_option_list_size = sizeof(long_option_list) / sizeof(*long_option_list);

    printf("Long options:");
    for (size_t i = 0; i < long_option_list_size; ++i) {
        if (mask & (1<<i)) printf(" '%s'", long_option_list[i]);
    }
    putc('\n', stdout);
}
static void print_non   (const char** non_option_list) {
    printf("Non options:");
    for (const char** opt = non_option_list; *opt != NULL; ++opt) {
        printf(" '%s'", *opt);
    }
    putc('\n', stdout);
}

int parse(int count, const char** args) {
    if (args == NULL) {
        LOG_R("args is NULL\n");
        return -1;
    }

    int short_mask = 0;
    int long_mask  = 0;
    const char** non_option_list = calloc(count + 1, sizeof(*non_option_list));

    const char* arg;
    int error = 0;

    for (int i = 0; i < count; ++i) {
        arg = args[i];

        if (arg == NULL) {
            LOG_R("arg[%d] is NULL\n", i);
            free(non_option_list);
            return -1;
        }

        if (arg[0] == '-') {
            if (arg[1] != '-') error = handle_short (arg + 1, &short_mask);
            else               error = handle_long  (arg + 2, &long_mask );
        } else                 error = handle_non   (arg, non_option_list);

        if (error) {
            free(non_option_list);
            return -1;
        }
    }

    print_short (short_mask     );
    print_long  (long_mask      );
    print_non   (non_option_list);

    free(non_option_list);

    return 0;
}
