#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


#define BUF_SIZE 1024

void checkArgs(int argc, char** argv) {
    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <infile> <outfile>\n", argv[0]);
        exit(1); 
    }
    return;
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

void printFile(char* infile, char* outfile) {
    int buf[BUF_SIZE];
    int bytesRead = 0;

    int i_fd = open(infile, O_RDONLY);
    int o_fd = open(outfile, O_WRONLY);
    if (i_fd == -1 || o_fd == -1) {
        perror("Open Failure");
        exit(1);
    }

    while ((bytesRead = read(i_fd, buf, BUF_SIZE)) != 0) {
        writeBytes(o_fd, buf, bytesRead);
    }
    if (bytesRead == -1) {
        perror("Read Failure");
        exit(1);
    }
}

int main(int argc, char** argv) {

    checkArgs(argc, argv);
    printFile(argv[1], argv[2]);

    return 0;
}
