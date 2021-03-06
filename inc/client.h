#ifndef _CLIENT_H
#define _CLIENT_H
#include "blokus.h"
#include "render.h"

#define S_CHOOSE_TILE 0
#define S_POSITIONING 1
#define S_PLACING 2

int choose_tile_handler(int c, rcb_t *rcb, char *msg[7], int *color[7]);
int positioning_handler(int c, rcb_t *rcb, char *msgs[7], int *color[7]);
int placing_handler(int c, rcb_t *rcb, char *msgs[7], int *color[7]);
int opponent_place_handler(rcb_t *rcb, char *code, char *msgs[7], int *color[7]);
int game_over_handler(rcb_t *rcb, char *msgs[7], int *color[7]);
#endif
