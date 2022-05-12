#ifndef _RENDER_H
#define _RENDER_H

#include "blokus.h"

extern const coord_t TILES_CURS[SHAPE_Z + 1];
extern const coord_t ALPHA_CURS[SHAPE_Z + 1];

int modify_board(gcb_t *gcb);
int modify_tiles(gcb_t *gcb, int player);
int modify_chosen_tile(gcb_t *gcb);
int modify_message_log(gcb_t *gcb);

#endif
