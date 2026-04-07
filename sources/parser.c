#include "parser.h"
#include "common.h"

static int handle_short (const char* arg, int* short_mask);
static int handle_long  (const char* arg, int* long_mask );
static int handle_none  (const char* arg, char** none_options_list);

static int print_short (int short_mask);
static int print_long  (int long_mask);
static int print_none  (char** none_options_list);

int parse(int count, const char** args) {
    if (args == NULL) {
        log_R("args is NULL\n");
        return -1;
    }

    const char* arg;
    int short_mask = 0;
    int long_mask  = 0;
    char** none_options_list = calloc(count, sizeof(*none_options_list) + 1);
    int error = 0;

    for (int i = 0; i < count; ++i) {
        arg = args[i];

        if (arg == NULL) {
            log_R("arg[%d] is NULL\n", i);
            free(none_options_list);
            return -1;
        }

        if (arg[0] == '-') {
            if (arg[1] == '-') error = handle_long  (arg + 2, &short_mask  );
            else               error = handle_short (arg + 1, &long_mask   );
        } else                 error = handle_none  (arg, none_options_list);

        if (error) {
            free(none_options_list);
            return -1;
        }
    }

    print_short (short_mask       );
    print_long  (long_mask        );
    print_none  (none_options_list);
}
