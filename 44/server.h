#ifndef SERVER_H_
#define SERVER_H_

#define SERVER_PIPE "/tmp/server_fifo"
#define MAX_FDS 10
#define BUF_LEN 1024
#define TYPE_LEN 256
#define ECHO "Echo"
#define PIPE "Pipe"
#define OK "OK"

typedef struct Message {
    char msg_buf[BUF_LEN];
    char type[256];
    unsigned int len;
} Message;

typedef struct FDS {
    int fds[MAX_FDS];
    int num_fds;
} FDS;

#endif
