#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


#define BUF_SIZE 1024

char* checkArgs(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: <prog> <filepath>\n");
        exit(1); 
    }
    return argv[1];
}

void writeBytes(int fd, int* buf, int num) {
    int bytesSoFar = 0;

    while (bytesSoFar < num) {
        int writeBytes = write(fd, buf, (num-bytesSoFar));
        if (writeBytes == -1) {
            perror("Write Failure");
            exit(1);
        }
        buf += writeBytes;
        bytesSoFar += writeBytes;
    }
}

void printFile(char* filepath) {
    int buf[BUF_SIZE];
    int bytesRead = 0;

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror(strerror(errno));
        exit(1);
    }

    while ((bytesRead = read(fd, buf, BUF_SIZE)) != 0) {
        if (bytesRead < 0) {
            perror("Read Failure");
            exit(1);
        }
        writeBytes(1, buf, bytesRead);
    }
}

int main(int argc, char** argv) {

    char* filepath = checkArgs(argc, argv);
    printFile(filepath);

    return 0;
}
