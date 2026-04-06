#ifndef __TEXT__
#define __TEXT__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "text.h"

#define MAX_ALLOWED_DATA_SIZE (2<<20)

typedef struct text_t Text;

struct text_t {
    char* buffer;
    char** strings;
    size_t buffer_size;
    size_t strings_size;
};

Text *create_text(const char *input_file);
int   delete_text(Text *text);

int send_text_to_file(Text *text, const char *output_file);
int sort_data(const char *input_file, const char *output_file, const char *cmp_name);

#endif//__TEXT__
