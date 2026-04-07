#ifndef __CHAT__
#define __CHAT__

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define MSG_BUFFER_SIZE 4096
#define TMP_POST ".tmp"
#define EXIT_MESSAGE "exit\n"

typedef struct chat_t Chat;

struct chat_t {
    int fd_read;
    int fd_write;
};

Chat* create_chat(const char* file_name);
int delete_chat(Chat* fifo);
int start_chatting(Chat* fifo);

#endif//__CHAT__
