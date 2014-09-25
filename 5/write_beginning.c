#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    if (argc != 2 || strncmp(argv[1], "--help", 6) == 0) {
        printf("Usage: %s <filepath>\n", argv[0]);
        exit(0);
    }

    int fd = open(argv[1], O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Open Error");
        exit(1);
    }

    off_t off = lseek(fd, 0, SEEK_SET);
    if (off == -1) {
        perror("Seek Error");
        exit(1);
    }

    ssize_t bytes = write(fd, "test", 4);
    if (bytes == -1) {
        perror("Write Error");
        exit(1);
    }
    
    return 0;
}
