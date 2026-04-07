#include "common.h"
#include "fork_copy.h"

static int execute_cat(int fd_in, int fd_out) {
    pid_t pid;
    int status;

    if ((pid = fork()) == -1) {
        LOG_R("cat fork failed\n");
        return -1;
    } else if (pid == 0) {
        dup2(fd_in,  STDIN_FILENO );
        dup2(fd_out, STDOUT_FILENO);
        execlp("cat", "cat", (char *)NULL);
        LOG_R("cat execlp failed\n");
        exit(-1);
    }
    waitpid(pid, &status, 0);

    return status == -1 ? -1 : 0;
}

static int parent_handler (int fd_in);
static int child_handler  (int fd_in);

static int print_parent_child() {
    int fd_p = open(PARENT_NAME, O_RDONLY);
    if (fd_p == -1) return -1;
    printf("Parent:\n");
    execute_cat(fd_p, STDOUT_FILENO);
    close(fd_p);

    int fd_c = open(PARENT_NAME, O_RDONLY);
    if (fd_c == -1) return -1;
    printf("Parent:\n");
    execute_cat(fd_c, STDOUT_FILENO);
    close(fd_c);

    return 0;
}

int fork_copy(const char* file_path) {
    if (file_path == NULL) {
        LOG_R("NULL file_path\n");
        return -1;
    }

    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        LOG_R("open file '%s' error\n", file_path);
        return -1;
    }

    int pid;
    if ((pid = fork()) == -1) {
        LOG_R("fork error\n");
        return -1;
    } else if (pid == 0) {
        int res = child_handler(fd);
        close(fd);
        exit(res);
    }

    int p_res = parent_handler(fd);
    close(fd);

    int c_res;
    waitpid(pid, &c_res, 0);

    if (c_res != 0 || p_res != 0) return -1;
    print_parent_child();
    return 0;
}
