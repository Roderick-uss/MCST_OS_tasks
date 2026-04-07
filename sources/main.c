#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *string_to_int_list (char *line, size_t *list_size) {
    size_t line_size = strlen(line);
    *list_size = 0;

    for(size_t i = 0, is_new_int = 1; i < line_size; ++i) {
        if (line[i] == ' ' ) {
            is_new_int = 1;
            line[i] = '\0';
        }
        else if (is_new_int) {
            (*list_size)++;
            is_new_int = 0;
        }
    }

    int *list = calloc(*list_size, sizeof(*list));
    if(list == NULL) return NULL;

    size_t curret_list_idx = 0;
    for(size_t i = 0; i < line_size; i += strlen(line + i) + 1) {
        list[curret_list_idx++] = strtol(line + i, NULL, 10);
    }

    return list;
}
int get_lis (int *list, size_t list_size, size_t *begin_idx, size_t *end_idx);

int main() {
    char *line;
    scanf("%m[-+0-9 ]", &line);

    size_t list_size;
    int *list = string_to_int_list(line, &list_size);

    free(line);
    if (list == NULL) return -1;

    size_t begin_idx = 0, end_idx = list_size;
    // get_lis(list, list_size, &begin_idx, &end_idx);

    for (size_t idx = begin_idx; idx < end_idx - 1; ++idx) {
        printf("%d ", list[idx]);
    }
    printf("%d\n", list[end_idx - 1]);

    free(list);

    return 0;
}
