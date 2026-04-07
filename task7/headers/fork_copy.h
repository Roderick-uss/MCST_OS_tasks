#ifndef __FORK_COPY__
#define __FORK_COPY__

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define CHILD_NAME  "child_copy"
#define PARENT_NAME "parent_copy"

// #define MAX_DATA_BUFFER_SIZE (1<<20)

// typedef struct data_buffer_t DataBuffer;

// struct data_buffer_t {
//     int size;
//     char *buffer;
// };


int fork_copy(const char* file_path);


#endif//__FORK_COPY__
