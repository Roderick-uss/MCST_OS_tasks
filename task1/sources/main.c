#include "thread_sort.h"
#include "common.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        LOG_R("ERROR: incorrect arguments number\n");
        return -1;
    }

    int thread_number = strtol(argv[1], NULL, 10);

    size_t size;
    int* list = get_list(&size);
    if (list == NULL) {
        return -1;
    }

    sort(list, size, thread_number);

    for (size_t i = 0; i < size; ++i) {
        printf("%d \n", list[i]);
    }
    printf("\n");

    free(list);
    return 0;
}
