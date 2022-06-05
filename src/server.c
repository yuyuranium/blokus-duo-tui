#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include "sock.h"
#include "frame.h"

#define STAT_INIT 0
#define STAT_WAITING 1
#define STAT_PAIRED 2
#define STAT_INGAME 3
#define STAT_EOG 4

typedef struct player player_t;
// typedef struct scb scb_t;

struct player {
    long clientfd;
    int status;
    int turn;
    player_t *opp;
    // scb_t *scb;
};

// May not need this
/*
struct scb {
    player_t *p[2];
    int turn;
    // may add more attribute
};  // service control block
*/

static pthread_mutex_t mutex;

static player_t *waiting_player = NULL;

static player_t *init_player(long clientfd)
{
    player_t *p = malloc(sizeof(player_t));
    p->clientfd = clientfd;
    // p->next = NULL;
    return p;
}

// static scb_t *init_scb(player_t *p, player_t *q)
// {
//     scb_t *scb = malloc(sizeof(scb_t));
//     return scb;
// }

// Seems queue is not needed
/*
typedef struct player_queue {
    player_t *head;
    player_t *tail;
} player_queue_t;

static player_queue_t waiting_queue;


static int enqueue(player_queue_t *q, player_t *p)
{
    if (!q->head && !q->tail) {
        q->tail = q->head = p;
    } else {
        q->tail->next = p;
        q->tail = p;
    }
    return 0;
}

static player_t *dequeue(player_queue_t *q)
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

static int rm_player(player_queue_t *q, player_t *p)
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
*/

static void die(char *msg)
{
    printf("%s\n", msg);
    exit(-1);
}

static void bad_request(long clientfd)
{
    char *res_frame = get_frame(-1, RES_BAD_REQ, NULL);
    send(clientfd, res_frame, FRAME_LEN, 0);
    free(res_frame);
    printf("info: client fd %ld: bad request\n", clientfd);
}

static void invalid(long clientfd)
{
    char *res_frame = get_frame(-1, RES_INV, NULL);
    send(clientfd, res_frame, FRAME_LEN, 0);
    free(res_frame);
    printf("info: client fd %ld: invalid\n", clientfd);
}

static void res(long clientfd, int opcode, char *code)
{
    char *res_frame = get_frame(opcode, RES_OK, code);
    send(clientfd, res_frame, FRAME_LEN, 0);
    free(res_frame);
    printf("info: client fd %ld: ok with opcode: %d", clientfd, opcode);
}

static void *serve(void *argp)
{
    long clientfd = (long) argp;
    player_t *p = init_player(clientfd);
    while (1) {
        char req_frame[FRAME_LEN];
        int req_sz = recv(clientfd, req_frame, FRAME_LEN, 0);
        if (req_sz == 0) {
            printf("info: connection fd %ld shutdowned\n", clientfd);
            break;
        }

        if (req_sz < 0) {
            printf("error: error occurred when communicating with fd %ld\n",
                   clientfd);
            break;
        }

        int opcode, status;
        char code[CODE_LEN];
        if (parse_frame(req_frame, &opcode, &status, code) < 0) {
            bad_request(clientfd);
            continue;
        }

        printf("info: client fd %ld: request: (%d, %d, ",
               clientfd, opcode, status);
        for (int i = 0; i < CODE_LEN; ++i)
            printf("(%x)", code[i]);
        printf(")\n");

        switch (opcode) {
        case REQ_PAIR:
            memset(code, 0, CODE_LEN);
            switch (p->status) {
            case STAT_INIT:
            case STAT_EOG:
                pthread_mutex_lock(&mutex);
                if (!waiting_player) {
                    p->status = STAT_WAITING;
                    p->turn = 0;
                    waiting_player = p;
                    res(clientfd, PAIRED, code);
                } else {
                    code[0] = 1;
                    p->status = STAT_PAIRED;
                    p->turn = 1;
                    waiting_player->status = STAT_PAIRED;
                    // bind p and waiting_player
                    p->opp = waiting_player;
                    waiting_player->opp = p;
                    waiting_player = NULL;
                    res(clientfd, PAIRED, code);
                }
                pthread_mutex_unlock(&mutex);
                break;
            case STAT_WAITING:
                res(clientfd, PAIRED, code);
                break;
            case STAT_PAIRED:
                code[0] = 1;
                res(clientfd, PAIRED, code);
                break;
            case STAT_INGAME:
                invalid(clientfd);
                break;
            }
            break;
        case REQ_TURN:
        case REQ_STATUS:
        case REQ_PLACE:
        default:
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
