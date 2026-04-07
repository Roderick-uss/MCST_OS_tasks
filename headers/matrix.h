#ifndef __MATRIX__
#define __MATRIX__

#include <unistd.h>
#include <signal.h>

#define MATRIX_SIZE 100
#define ARG_SIGNAL    "--signal"
#define ARG_SIGACTION "--sigaction"

enum signal_type_t {
    TYPE_SIGNAL=0,
    TYPE_SIGACTION=1,
    TYPE_NONE=2,
};

typedef struct matrix_iterator_t MatrixIterator;

struct matrix_iterator_t {
    int row;
    int col;
};


int calculate_matrix_signal(enum signal_type_t sig);

#endif//__MATRIX__
