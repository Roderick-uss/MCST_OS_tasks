#include "chat.h"
#include "common.h"

int main(int argc, const char** argv) {
    if (argc != 2) {
        LOG_R("ERROR: exactly one argument requider\n");
        return -1;
    }
    Chat *chat = create_chat(argv[1]);
    start_chatting(chat);
    delete_chat(chat);
    return 0;
}
