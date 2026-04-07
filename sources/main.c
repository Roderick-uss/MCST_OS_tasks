#include "common.h"
#include "fork_copy.h"

int main(int argc, const char** argv) {
    if (argc != 2) {
        LOG_R("wrong number of parameters\n");
        return -1;
    }
    return fork_copy(argv[1]);
}
