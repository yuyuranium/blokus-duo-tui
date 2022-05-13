#ifndef _RENDER_H
#define _RENDER_H

#include "blokus.h"

extern const coord_t TILES_CURS[SHAPE_Z + 1];
extern const coord_t ALPHA_CURS[SHAPE_Z + 1];

// Color pair
#define CYAN_PAIR 1

int render_board(gcb_t *gcb);
int render_tiles(gcb_t *gcb, int player);
int render_chosen_tile(tile_t *tile);
int render_message_log(char *msg[6], int len[6]);
int shift_msg(char *msg[6], int len[6]);

#endif
