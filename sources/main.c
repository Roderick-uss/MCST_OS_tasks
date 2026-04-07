#include "common.h"
#include "parser.h"

int main(int argc, char** argv) {
    return parse(argc - 1, argv + 1);
}
