#ifndef __SORT__
#define __SORT__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef int (*cmp_t) (const void *first, const void *second);

cmp_t get_comparator(const char *name);

int sort_data(const char *input_file, const char *output_file, const char *cmp_name);

#endif//__SORT__
