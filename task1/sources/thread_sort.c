#include "thread_sort.h"
#include "common.h"

static void swap_int(int* first, int* second) {
    if (first == NULL || second == NULL) {
        LOG_R("ERROR: tried to swap 0 pointer\n");
        return;
    }
    if (*first == *second) return;
    *first += *second;
    *second = *first - *second;
    *first  = *first - *second;
}

static int partition(int* list, task_t* task, task_t* left_task, task_t* right_task) {
    if (list == NULL) {
        LOG_R("ERROR: partition list not exist\n");
        return -1;
    } else if (task == NULL) {
        LOG_R("ERROR: no task in partition\n");
        return -1;
    } else if (left_task == NULL || right_task == NULL) {
        LOG_R("ERROR: no task for output in partition\n");
        return -1;
    }

    int *begin, *end;
    int left_border, right_border;

    begin = list + task->index;
    end   = begin + task->size;
    left_border  = -1;
    right_border = end - begin;

    int pivot = begin[0];

    while (1) {
        while (begin[++left_border ] < pivot);
        while (begin[--right_border] > pivot);

        if (left_border > right_border) break;
        swap_int(begin + right_border, begin + left_border);
    }

    left_task->size  = left_border;
    left_task->index = task->index;

    right_task->size  = task->size  - left_border;
    right_task->index = task->index + left_border;

    return 0;
}

static int copy_task(task_t* task1, task_t* task2) {
    if (task1 == NULL || task2 == NULL) {
        LOG_R("ERROR: tried to copy 0 pointer\n");
        return -1;
    }
    task1->index = task2->index;
    task1->size = task2->size;
    return 0;
}

static int add_task(thread_pool_t* pool, task_t* task) {
    pthread_mutex_lock(&pool->mutex);

    copy_task(&pool->task_list[pool->task_list_size], task);
    pool->task_list_size++;
    pthread_cond_signal(&pool->cond);

    pthread_mutex_unlock(&pool->mutex);
    return 0;
}
static int get_task(thread_pool_t* pool, task_t* task) {
    pthread_mutex_lock(&pool->mutex);

    pool->thread_waiting++;
        while (pool->task_list_size == 0) {
            if (pool->thread_waiting == pool->thread_count) {
                pthread_cond_signal(&pool->cond);
                pthread_mutex_unlock(&pool->mutex);

                task->index = -1;
                return 0;
            }

            pthread_cond_wait(&pool->cond, &pool->mutex);
        }
    pool->thread_waiting--;

    pool->task_list_size--;
    copy_task(task, &pool->task_list[pool->task_list_size]);

    pthread_mutex_unlock(&pool->mutex);
    return 0;
}
static void *thread_action(void* args) {
    if (args == NULL) {
        LOG_R("ERROR: no args in thread action\n");
        pthread_exit(NULL);
    }
    thread_pool_t* pool = (thread_pool_t*)args;
    task_t task, left_task, right_task;
    while(1) {
        get_task(pool, &task);

        if (task.index == -1) {
            break;
        }

        while (task.size > 1) {
            partition(pool->list, &task, &left_task, &right_task);
            copy_task(&task, &left_task);
            if (right_task.size > 1) {
                add_task(pool, &right_task);
            }
        }
    }
    pthread_exit(0);
}

static int init_thread_pool(thread_pool_t* pool, int thread_count, int* list, size_t size) {
    pool->list = list;
    pool->list_size = size;

    pool->task_list = calloc(size, sizeof(*pool->task_list));
    pool->task_list_size = 0;

    pool->thread_count = thread_count;
    pool->thread_list = calloc(thread_count, sizeof(*pool->thread_list));
    pool->thread_waiting = 0;


    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init (&pool->cond,  NULL);

    return 0;
}
static int destroy_thread_pool(thread_pool_t* pool) {
    free(pool->task_list);
    free(pool->thread_list);

    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy (&pool->cond );
    return 0;
}

int sort(int* list, size_t size, int thread_count) {
    if (!list) {
        LOG_R("ERROR: list not found\n");
        return -1;
    }
    if (thread_count > MAX_THREAD_COUNT) {
        LOG_Y("WARNING: thread count %d is too big used %d instdead\n", thread_count, MAX_THREAD_COUNT);
        thread_count = MAX_THREAD_COUNT;
    }
    if (size == 0) {
        LOG_Y("WARNING: size is ""\033[36m""0\n");
        return -1;
    }

    thread_pool_t pool;
    task_t task;

    task.index = 0;
    task.size = size;

    init_thread_pool(&pool, thread_count, list, size);
    add_task(&pool, &task);

    for (int th = 0; th < thread_count; ++th) {
        pthread_create(pool.thread_list + th, NULL, thread_action, &pool);
    }

    for (int th = 0; th < thread_count; ++th) {
        pthread_join(pool.thread_list[th], NULL);
    }

    destroy_thread_pool(&pool);
    return 0;
}

int* get_list(size_t* size_) {
    size_t size = 0;
    int* list = calloc(MAX_LINE_SIZE, sizeof(int));
    char line[MAX_LINE_SIZE];

    fgets(line, MAX_LINE_SIZE, stdin);
    line[strlen(line) - 1] = '\0';

    char* token = strtok(line, DELIM);
    char* endptr = NULL;

    while (token != NULL) {
        if (size == MAX_LIST_SIZE) {
            LOG_R("ERROR: too many numbers in list\n");
            free(list);
            return NULL;
        }

        list[size++] = strtol(token, &endptr, 10);

        if (*endptr != '\0') {
            LOG_R("ERROR: not a number: ""\033[36m""%s\n", token);
            free(list);
            return NULL;
        }

        token = strtok(NULL, DELIM);
    }

    *size_ = size;
    return list;
}
