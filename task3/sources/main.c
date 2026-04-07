#include "sort.h"
#include "common.h"

int main(int argc, char** argv) {
    error_wrap(argc != 4, "ERROR: must be 3 arguments\n",, -1);
    if (sort_data(argv[1], argv[2], argv[3]) == -1) {
        LOG_R("sort data failed\n");
    }
    return 0;
}
