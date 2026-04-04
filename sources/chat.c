#include "chat.h"
#include "common.h"

//could be added signal handler of sigint to delete chat on exit

static int is_chat(const char* file_name) {
    struct stat file_info;
    if (stat(file_name, &file_info) == -1) {
        LOG_R("ERROR: fstat failed\n");
        return -1;
    }
    return S_ISFIFO(file_info.st_mode);
}

static void *loop_write(void* args) {
    Chat* chat = args;
    if (chat == NULL) {
        LOG_R("ERROR: no chat in loop_write\n");
        pthread_exit(NULL);
    }

    int fd = chat->fd_write;

    char msg[MSG_BUFFER_SIZE];
    int size;

    while (1) {
        fgets(msg, MSG_BUFFER_SIZE, stdin);
        size = strlen(msg);

        if (msg[size - 1] != '\n') {
            LOG_R("ERROR: too long message\n");
            pthread_exit(NULL);
        }

        if (strcmp(msg, EXIT_MESSAGE) == 0) {
            break;
        }

        write(fd, msg, size);
    }

    pthread_exit(NULL);
}

static void *loop_read(void* args) {
    Chat* chat = args;
    if (chat == NULL) {
        LOG_R("ERROR: no chat in loop_read\n");
        pthread_exit(NULL);
    }

    int fd = chat->fd_read;

    char msg[MSG_BUFFER_SIZE];
    int size, delta;

    while (1) {
        SET_BLOCK(fd);
        size = read(fd, msg, 1);
        SET_NONBLOCK(fd);

        while ((delta = read(fd, msg + size, MSG_BUFFER_SIZE - size)) != -1) {
            size += delta;
        };
        if (errno != EAGAIN) {
            LOG_R("ERROR: read fifo error\n");
            pthread_exit(NULL);
        }
        if (msg[size - 1] != '\n') {
            LOG_R("ERROR: recied strange message\n");
            pthread_exit(NULL);
        }

        write(STDOUT_FILENO, msg, size);
    }

    pthread_exit(NULL);
}

Chat* create_chat(const char* file_name) {
    if (file_name == NULL) {
        LOG_R("ERROR: no file name given\n");
        return NULL;
    }
    char* tmp_file_name = calloc(sizeof(char), strlen(file_name) + strlen(TMP_POST));
    strcat(tmp_file_name, file_name);
    strcat(tmp_file_name, TMP_POST);

    if (mkfifo(tmp_file_name, 0666) == -1 && errno != EEXIST) {
        LOG_R("ERROR: tmp chat failed to create\n");
        return NULL;
    }

    if (!is_chat(file_name)){
        LOG_R("ERROR: given file is not chat\n");
        return NULL;
    }

    Chat* chat;
    CALLOC_SELF(chat);


    chat->fd_write = open(file_name, O_WRONLY | O_NONBLOCK);

    if (chat->fd_write != -1) {
        SET_BLOCK(chat->fd_write);
        chat->fd_read = open(tmp_file_name, O_RDONLY | O_NONBLOCK);
    }
    else if (chat->fd_write == -1 && errno == ENXIO) {
        chat->fd_read = open(file_name, O_RDONLY | O_NONBLOCK);
        chat->fd_write = open(tmp_file_name, O_WRONLY);
        SET_BLOCK(chat->fd_write);
    }
    else {
        LOG_R("ERROR: can not open chat\n");
        delete_chat(chat);
        return NULL;
    }

    unlink(tmp_file_name);

    return chat;
}

int delete_chat(Chat* chat) {
    if (chat == NULL) {
        return 0;
    }

    close(chat->fd_read);
    close(chat->fd_write);



    memset(chat, 0, sizeof *chat);
    free(chat);
    return 0;
}

int start_chatting(Chat* chat) {
    if (chat == NULL) {
        LOG_R("ERROR: no chat given\n");
        return -1;
    }

    pthread_t reader, writer;

    pthread_create(&writer, NULL, loop_write, chat);
    pthread_create(&reader, NULL, loop_read , chat);

    pthread_join(writer, NULL);
    pthread_cancel(reader);

    return 0;
}
