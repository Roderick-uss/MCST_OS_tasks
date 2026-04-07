#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *string_to_int_list (char *line, size_t *list_size);
int get_lis (int *list, size_t list_size, size_t *begin_idx, size_t *end_idx);

int main() {
    char *line;
    scanf("%m[-+0-9 ]", &line);

    size_t list_size;
    int *list = string_to_int_list(line, &list_size);

    free(line);
    if (list == NULL) return -1;

    size_t begin_idx, end_idx;
    get_lis(list, list_size, &begin_idx, &end_idx);

    for (int idx = begin_idx + 1; idx < end_idx - 1; ++idx) {
        printf("%d ", list[idx]);
    }
    printf("%d\n", list[end_idx - 1]);

    free(list);

    return 0;
}
