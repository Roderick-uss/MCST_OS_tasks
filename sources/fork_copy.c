#include "common.h"
#include "fork_copy.h"

static int parent_handler (int fd);
static int child_handler  (int fd);

int fork_copy(const char* file_path);
