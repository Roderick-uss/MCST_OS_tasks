#include "common.h"
#include "matrix.h"

static MatrixIterator i_A = {}, i_B = {};

static void handler(int) {
    printf("\n%2d %2d\n""%2d %2d\n""%2d %2d\n",
           i_A.row, i_A.col,
           i_B.row, i_B.col,
           i_A.row, i_B.col);
    signal(SIGINT, SIG_DFL);
}

static void handler_sa(int, siginfo_t, void*) {
    printf("\n%2d %2d\n""%2d %2d\n""%2d %2d\n",
           i_A.row, i_A.col,
           i_B.row, i_B.col,
           i_A.row, i_B.col);

    struct sigaction sa_parameters = {
            .sa_mask = SIGINT,
            .sa_handler = SIG_DFL,
    };
    sigaction(SIGINT, &sa_parameters, NULL);
}


int calculate_matrix(enum signal_type_t sig) {
    if (sig == TYPE_SIGNAL) {
        signal(SIGINT, handler);
    } else if (sig == TYPE_SIGACTION) {
        struct sigaction sa_parameters = {
            .sa_mask = SIGINT,
            .sa_flags = SA_SIGINFO,
            .sa_sigaction = handler_sa,
        };
        sigaction(SIGINT, &sa_parameters, NULL);
    } else {
        return -1;
    }

    int A[MATRIX_SIZE][MATRIX_SIZE], B[MATRIX_SIZE][MATRIX_SIZE], AB[MATRIX_SIZE][MATRIX_SIZE];
    for (i_A.row = 0; i_A.row < MATRIX_SIZE; ++(i_A.row)) {
        for (i_B.col = 0; i_B.col < MATRIX_SIZE; ++(i_B.col)) {
            for (i_A.col = i_B.row = 0, AB[i_A.row][i_B.col] = 0; i_A.col < MATRIX_SIZE; ++(i_A.col), ++(i_B.row)) {
                AB[i_A.row][i_B.col] += A[i_A.row][i_A.col] * B[i_B.row][i_B.col];
                usleep(500000);
            }
        }
    }

    return 0;
}
