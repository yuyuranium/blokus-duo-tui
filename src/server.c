#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include "sock.h"

int main(int argc, char *argv[])
{
    char *server_port = NULL;
    --argc; ++argv;
    if (argc > 0 && **argv == '-' && (*argv)[1] == 'p') {
        --argc; ++argv;
        if (argc < 1) {
            printf("error: No port number provided\n");
            return -1;
        }
        server_port = malloc(strlen(*argv) + 1);
        strncpy(server_port, *argv, strlen(*argv));
        --argc; ++argv;
    }
    if (argc < 1) {
        printf("usage: servr -p server_port");
        return -1;
    }

    return 0;
}
