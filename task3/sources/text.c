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

static int set_buffer(Text *text, const char* input_file) {
    error_wrap(input_file == NULL, "ERROR: set buffer; input file is NULL\n",,  -1);
    error_wrap(text       == NULL, "ERROR: set buffer; text is NULL\n",,        -1);

    int size = get_file_size(input_file);
    error_wrap(size == -1,"",, -1);

    text->buffer_size = size;
    text->buffer = calloc(text->buffer_size + 1, sizeof(*text->buffer));
    error_wrap(text->buffer == NULL, "ERROR: set buffer; calloc text buffer error\n",, -1);

    int fd = open(input_file, O_RDONLY);
    error_wrap(fd == -1, "ERROR: set buffer; open input file error\n",, -1);

    size = read(fd, text->buffer, text->buffer_size);
    close(fd);
    error_wrap(size == -1,                    "ERROR: set buffer; read error\n",, -1);
    error_wrap((size_t)size != text->buffer_size, "ERROR: set buffer; read less\n",,  -1);

    return 0;
}
static int set_strings(Text *text) {
    error_wrap(text         == NULL, "ERROR: set strings; text is NULL\n",,        -1);
    error_wrap(text->buffer == NULL, "ERROR: set strings; text buffer is NULL\n",, -1);

    text->strings_size = 0;

    int prev_is_new_line;
    char* current;
    size_t size = 0;

    prev_is_new_line = 1;
    current = text->buffer - 1;

    while (++current != text->buffer + text->buffer_size) {
        if (*current == '\n') {
            *current = '\0';
            prev_is_new_line = 1;
        } else if (prev_is_new_line) {
            size++;
            prev_is_new_line = 0;
        }
    }

    text->strings = calloc(size, sizeof(*text->strings));
    error_wrap(text->strings == NULL, "ERROR: set strings; calloc text strings error\n",, -1);

    prev_is_new_line = 1;
    current = text->buffer - 1;
    while (++current != text->buffer + text->buffer_size) {
        if (*current == '\0') {
            prev_is_new_line = 1;
        } else if (prev_is_new_line) {
            text->strings[text->strings_size++] = current;
            prev_is_new_line = 0;
        }
    }

    return 0;
}

Text *create_text(const char *input_file) {
    error_wrap(input_file == NULL,
        "ERROR: create text; input file is NULL\n",, NULL);

    Text *text = calloc(1, sizeof(*text));

    error_wrap(text  == NULL, "ERROR: calloc text error\n", delete_text(text), NULL);
    error_wrap(set_buffer(text, input_file) == -1, "",      delete_text(text), NULL);
    error_wrap(set_strings(text)            == -1, "",      delete_text(text), NULL);

    return text;
}

int   delete_text(Text *text) {
    if (text == NULL) return 0;

    int retval = 0;

    if (text->buffer  != NULL) {
        free(text->buffer);
    } else {
        LOG_R("ERROR: delete text; text buffer is NULL\n");
        retval = -1;
    }
    if (text->strings != NULL) {
        free(text->strings);
    } else {
        LOG_R("ERROR: delete text; text strings is NULL\n");
        retval = -1;
    }

    memset(text, 0, sizeof(*text));
    free(text);

    return retval;
}

int send_text_to_file(Text *text, const char *output_file) {
    error_wrap(output_file   == NULL, "ERROR: send text to file; output file is NULL\n",,  -1);
    error_wrap(text          == NULL, "ERROR: send text to file; text is NULL\n",,         -1);
    error_wrap(text->strings == NULL, "ERROR: send text to file; text strings is NULL\n",, -1);

    int fd = open(output_file, O_RDWR | O_CREAT | O_TRUNC, 0666);
    error_wrap(fd == -1, "ERROR: send text to file; open output file error\n",, -1);

    char** current = text->strings - 1;
    while(++current != text->strings + text->strings_size) {
        size_t size = strlen(*current);
        (*current)[size] = '\n';
        write(fd, *current, size + 1);
        (*current)[size] = '\0';
    }
    close(fd);

    return 0;
}
