#ifndef __PRSER__
#define __PRSER__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char short_option_list[] = "mcst";
const char * const long_option_list[] = {"elbrus=1c+" "elbrus=2c+", "elbrus=2c3", "elbrus=4c", "elbrus=8c", "elbrus=16c"};

int parse(int count, const char** args);

#endif//__PRSER__
