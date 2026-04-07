#ifndef __THREAD_SORT___
#define __THREAD_SORT___

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

#define MAX_LINE_SIZE 100
#define MAX_LIST_SIZE 50
#define MAX_THREAD_COUNT 20
#define DELIM " "

typedef struct {
    int index;
    int size;
} task_t;

typedef struct {
    int* list;
    size_t list_size;

    task_t* task_list;
    size_t task_list_size;

    pthread_t* thread_list;
    int thread_waiting;
    int thread_count;

    pthread_cond_t cond;
    pthread_mutex_t mutex;
} thread_pool_t;

int sort(int* list, size_t size, int thread_count);
int* get_list(size_t* size_);


#endif//__THREAD_SORT___
