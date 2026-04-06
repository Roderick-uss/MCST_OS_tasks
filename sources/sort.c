#include "sort.h"
#include "common.h"
#include "text.h"

static int cmp_plain  (const void *first, const void *second) {
    error_wrap(first == NULL || second == NULL,
        "ERROR: NULL pointer in plain comparator\n", NULL, 0);

    return strcmp(*(const char *const*)first, *(const char *const*)second);
}
static int cmp_rplain (const void *first, const void *second) {
    error_wrap(first == NULL || second == NULL,
        "ERROR: NULL pointer in rplain comparator\n", NULL, 0);

    return -cmp_plain(first, second);
}
static int cmp_lex    (const void *first, const void *second) {
    error_wrap(first == NULL || second == NULL,
        "ERROR: NULL pointer in lex comparator\n", NULL, 0);

    const char *first_  = *(const char *const *)first;
    const char *second_ = *(const char *const *)second;

    while(tolower(*first_) == tolower(*second_) && *first_ != 0) {
        first_++;
        second_++;
    }

    return tolower(*first_) - tolower(*second_);
}
static int cmp_rlex   (const void *first, const void *second) {
    error_wrap(first == NULL || second == NULL,
        "ERROR: NULL pointer in rlex comparator\n", NULL, 0);

    return -cmp_lex(first, second);
}

typedef struct cmp_type_t CmpType;
struct cmp_type_t {
    const char* name;
    cmp_t cmp;
};

const CmpType cmp_list[] = {
{"plain",  cmp_plain },
{"rplain", cmp_rplain},
{"lex",    cmp_lex   },
{"rlex",   cmp_rlex  },
};

cmp_t get_comparator(const char *name) {
    error_wrap(name == NULL, "ERROR cmp name is NULL\n", NULL, NULL);

    for (size_t i = 0; i < sizeof(cmp_list) / sizeof(*cmp_list); ++i) {
        const CmpType *cmp_type = cmp_list + i;

        if (strcmp(name, cmp_type->name) == 0)
            return cmp_type->cmp;
    }

    printf("name""\033[36m"" %s ""\033[0m""is not in list of comparators\n", name);

    return NULL;
}

int sort_data(const char *input_file, const char *output_file, const char *cmp_name) {
    error_wrap(input_file  == NULL, "ERROR: input file is NULL\n",,      -1);
    error_wrap(output_file == NULL, "ERROR: output file is NULL\n",,     -1);
    error_wrap(cmp_name    == NULL, "ERROR: comparator name is NULL\n",, -1);

    cmp_t cmp = get_comparator(cmp_name);
    error_wrap(cmp == NULL, "",, -1);

    Text *text = create_text(input_file);
    error_wrap(text == NULL, "",, -1);

    qsort(text->strings, text->strings_size, sizeof(*text->strings), cmp);

    error_wrap(send_text_to_file(text, output_file) == -1, "", delete_text(text), -1);

    delete_text(text);
    return 0;
}
