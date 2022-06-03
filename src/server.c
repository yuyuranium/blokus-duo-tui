#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include "sock.h"

#define STAT_WAITING 0
#define STAT_INGAME 1

typedef struct player {
    long clientfd;
    int status;
    struct player *next;
} player_t;

typedef struct waiting_queue {
    player_t *head;
    player_t *tail;
} waiting_queue_t;

static pthread_mutex_t mutex;

player_t *init_player(long clientfd)
{
    player_t *p = malloc(sizeof(player_t));
    p->clientfd = clientfd;
    p->next = NULL;
    return p;
}

int enqueue(waiting_queue_t *q, player_t *p)
{
    if (!q->head && !q->tail) {
        q->tail = q->head = p;
    } else {
        q->tail->next = p;
        q->tail = p;
    }
    return 0;
}

player_t *dequeue(waiting_queue_t *q)
{
    player_t *tmp = q->head;
    if (tmp) {
        if (q->head == q->tail)  // only one player in the queue
            q->head = q->tail = NULL;
        else
            q->head = q->head->next;
        tmp->next = NULL;  // disconnect the player from the queue
    }
    return tmp;
}

int rm_player(waiting_queue_t *q, player_t *p)
{
    player_t **indirect = &q->head;
    while (*indirect != p)
        indirect = &(*indirect)->next;

    if (!*indirect)
        return -1;

    *indirect = p->next;
    if (q->head == NULL)
        q->tail = NULL;
    p->next = NULL;
    return 0;
}

static void die(char *msg)
{
    printf("%s\n", msg);
    exit(-1);
}

static void *serve(void *argp)
{
    long clientfd = (long) argp;
    player_t *p = init_player(clientfd);
    while (1) {
        char req_buf[128];
        int req_sz = recv(clientfd, req_buf, 128, 0);
        if (req_sz == 0) {
            printf("info: connection fd %ld shutdowned\n", clientfd);
            break;
        }

        if (req_sz < 0) {
            printf("error: error occurred when communicating with fd %ld\n",
                   clientfd);
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    char *server_port = NULL;
    --argc; ++argv;
    if (argc > 0 && **argv == '-' && (*argv)[1] == 'p') {
        --argc; ++argv;
        if (argc < 1)
            die("error: No port number provided\n");

        server_port = malloc(strlen(*argv) + 1);
        strncpy(server_port, *argv, strlen(*argv));
        --argc; ++argv;

        if (argc > 0)
            die("error: too many arguments");
    } else {
        die("usage: servr -p server_port\n");
    }

    int listenfd __attribute__((unused)) = open_listenfd(server_port);
    printf("listening on the port %s\n", server_port);

    pthread_mutex_init(&mutex, NULL);
    while (1) {
        struct sockaddr client_info;
        socklen_t addrlen;
        long clientfd = accept(listenfd, &client_info, &addrlen);
        pthread_t thread;
        pthread_create(&thread, NULL, serve, (void *)clientfd);
        printf("info: thread %ld created, serving connection fd %ld\n",
               (long)thread, clientfd);
    }

    return 0;
}
