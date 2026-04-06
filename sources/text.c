#include "text.h"
#include "common.h"

static int get_file_size(const char *file) {
    error_wrap(file == NULL, "ERROR: get file size; file is NULL\n",, -1);

    struct stat statbuf;
    error_wrap(stat(file, &statbuf) == -1, "ERROR: get file size; stat error\n",, -1);

    int size = statbuf.st_size;
    error_wrap(size > MAX_ALLOWED_DATA_SIZE, "ERROR: file size is too big\n",, -1);

    return size;
}
static int set_strings(Text *text) {
    error_wrap(text == NULL,
        "ERROR: set strings; text is NULL\n",, -1);
    error_wrap(text->buffer == NULL,
        "ERROR: set strings; text buffer is NULL\n",, -1);
    text->strings_size = 0;
    return 0;
}

Text *create_text(const char *input_file) {
    error_wrap(input_file == NULL,
        "ERROR: create text; input file is NULL\n",, NULL);

    int size;
    error_wrap((size = get_file_size(input_file)) == -1,"",, NULL);

    Text *text;

    error_wrap((text = calloc(sizeof(*text), 1)) == NULL,
        "ERROR: calloc text error\n", delete_text(text), NULL);

    text->buffer_size = size;
    error_wrap((text->buffer = calloc(sizeof(*text->buffer), text->buffer_size)) == NULL,
        "ERROR: calloc text buffer error\n", delete_text(text), NULL);

    error_wrap(set_strings(text) == -1, "", delete_text(text), NULL);

    return text;
}

int   delete_text(Text *text) {
    if (text == NULL) return 0;

    int retval = 0;

    if (text->buffer  == NULL) {
        free(text->buffer);
    } else {
        LOG_R("ERROR: delete text; text buffer if NULL\n");
        retval = -1;
    }
    if (text->strings == NULL) {
        free(text->strings);
    } else {
        LOG_R("ERROR: delete text; text strings if NULL\n");
        retval = -1;
    }

    memset(text, 0, sizeof(*text));
    free(text);

    return retval;
}
