#ifndef _BLOKUS_H
#define _BLOKUS_H 

#define N_ROW 14
#define N_COL 14

#define within_map(y, x) ((y) >= 0 && (y) < N_ROW && (x) >= 0 && (x) < N_COL)

typedef enum {
    // monomino
    SHAPE_O,
    // domino
    SHAPE_R,
    // trominoes
    SHAPE_M,
    SHAPE_A,
    // tetrominoes
    SHAPE_E,
    SHAPE_J,
    SHAPE_S,
    SHAPE_Q,
    SHAPE_H,
    // pentominoes
    SHAPE_F,
    SHAPE_I,
    SHAPE_L,
    SHAPE_N,
    SHAPE_P,
    SHAPE_T,
    SHAPE_U,
    SHAPE_V,
    SHAPE_W,
    SHAPE_X,
    SHAPE_Y,
    SHAPE_Z,
} shape_t;

typedef struct {
    int y;
    int x;
} coord_t;

typedef struct {
    shape_t shape;
    coord_t *blks;
    int blk_cnt;
    int can_mir;
    int rot_cnt;
} tile_t;

typedef struct {
    int turn;
    int hand[2][SHAPE_Z + 1];
    int score[2];
    int next_empty[N_ROW * N_COL];
    int prev_empty[N_ROW * N_COL];
    int map[N_ROW][N_COL];
} gcb_t;  // game control block

const coord_t STARTING_POINT[2] = {
    {.y = 0x9, .x = 0x4},
    {.y = 0x4, .x = 0x9}
};

const coord_t CORNER[4] = {
    {.y = -1, .x = -1},
    {.y = -1, .x = 1},
    {.y = 1, .x = -1},
    {.y = 1, .x = 1},
};

const coord_t EDGE[4] = {
    {.y = -1, .x = 0},
    {.y = 0, .x = -1},
    {.y = 0, .x = 1},
    {.y = 1, .x = 0},
};

gcb_t *init_gcb();
int test_place(gcb_t *gcb, tile_t *tile, coord_t coord);
tile_t *make_tile(shape_t shape);
int rot_tile(tile_t *tile, int theta);
int mir_tile(tile_t *tile);

#endif
