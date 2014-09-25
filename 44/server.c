#include "server.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/uio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/signal.h>
#include <errno.h>

int setupPipe() {
    umask(0);
    
    if (mkfifo(SERVER_PIPE, S_IRUSR | S_IWUSR | S_IWGRP) == -1
        && errno != EEXIST) {
        perror("Create Pipe");
        exit(1);
    } 
    int server_fd = open(SERVER_PIPE, O_RDONLY);
    if (server_fd == -1) {
        perror("Open Read Pipe");
        exit(1);
    }

    int dummy_fd = open(SERVER_PIPE, O_WRONLY);
    if (dummy_fd == -1) {
        perror("Open Write Pipe");
        exit(1);
    }

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("Signal");
        exit(1);
    }

    return server_fd;
}

Message readRequest(int server_fd) {
    int bytes = 0, offset = 0; 
    Message msg;
    while ((bytes = read(server_fd, &msg+offset, sizeof(Message)-offset)) > 0) {
        offset += bytes;
    }
    if (bytes == -1 || offset != sizeof(Message)) {
        perror("Request Read Error");
        exit(1);
    }
    return msg;
}

void writeResponse(int client_fd, char* response, unsigned len) {
    int bytes = 0, offset = 0;
    while ((bytes = write(client_fd, response+offset, len-offset)) > 0) {
        offset += bytes;
    }

    if (bytes == -1 || offset != len) {
        perror("Response Write Error");
        exit(1);
    }
}

void service(int server_fd) {
    for(;;) {
        Message msg = readRequest(server_fd);
        if (strncmp(msg.type, PIPE, 4) == 0) {
            char client_pipe[BUF_LEN];
            strncpy(client_pipe, msg.msg_buf, BUF_LEN);
            int client_fd = open(client_pipe, O_WRONLY);
            writeResponse(client_fd, OK, 2);
        } else {
            printf("Couldn't decipher message\n");
        }
    }
}

int main(int argc, char** argv) {
    int fd = setupPipe();
    service(fd);

    return 0;
}


