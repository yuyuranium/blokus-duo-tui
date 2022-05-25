#ifndef _CLIENT_H
#define _CLIENT_H
#include "blokus.h"
#include "render.h"

#define S_CHOOSE_TILE 0
#define S_POSITIONING 1

int choose_tile_handler(int c, rcb_t *rcb, coord_t *coord);
int positioning_handler(int c, rcb_t *rcb);

#endif
