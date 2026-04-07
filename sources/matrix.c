#include "common.h"
#include "matrix.h"

static MatrixIterator i_A = {}, i_B = {}, i_AB = {};

static void signal_handler(int) {
    signal(SIGINT, SIG_IGN);
    printf("%d %d\n""%d %d\n""%d %d\n",
           i_A.row,  i_A.col,
           i_B.row,  i_B.col,
           i_AB.row, i_AB.col);
    signal(SIGINT, SIG_DFL);
}


int calculate_matrix(enum signal_type_t sig);
