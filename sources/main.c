#include<string.h>

#include "common.h"
#include "matrix.h"

int main (int argc, const char** argv) {
    if (argc != 2) {
        LOG_R("wrong number of parameters\n");
        return -1;
    }

    enum signal_type_t sig;

    if      (strcmp(argv[1], ARG_SIGNAL   ) == 0) sig = TYPE_SIGNAL;
    else if (strcmp(argv[1], ARG_SIGACTION) == 0) sig = TYPE_SIGACTION;
    else {
        LOG_R("wrong parameter '%s'\n", argv[1]);
        return -1;
    }

    calculate_matrix(sig);
}
