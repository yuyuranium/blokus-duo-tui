#ifndef _RENDER_H
#define _RENDER_H

#include "blokus.h"

extern const coord_t TILES_CURS[SHAPE_Z + 1];
extern const coord_t ALPHA_CURS[SHAPE_Z + 1];
extern const shape_t tile_relation[4][6];
// Color pair
#define CYAN_PAIR 1
#define RED_PAIR 2
#define GREEN_PAIR 3
#define YELLOW_PAIR 4
#define BLUE_PAIR 5

#define MAX_LOG_LEN 53

typedef struct {
    gcb_t *gcb;
    int render_player;
    int state;
    tile_t *chosen;
    coord_t coord;
} rcb_t;  // render control block

int init_all_colors();
int render_board(gcb_t *gcb);
int render_board_preview(rcb_t *rcb);
int recover_board_preview(rcb_t *rcb);
int render_tiles(gcb_t *gcb, int player);
int render_tile_preview(gcb_t *gcb, shape_t shape);
int recover_tile_preview(gcb_t *gcb, shape_t shape);
int render_message_log(char *msg[7], int *color[7]);
int render_score_board();
int render_score(rcb_t *rcb, int first_in);
int shift_msg(char *msg[7], int *color[7]);

#endif
