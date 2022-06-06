#ifndef _SOCK_H
#define _SOCK_H

#define TIMEOUT 1000000

int open_clientfd(char *hostname, char *port) __attribute__((unused));
int open_listenfd(char *port) __attribute__((unused));

#endif
