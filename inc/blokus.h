#ifndef _BLOKUS_H
#define _BLOKUS_H 

#define P1 'o'
#define P2 'x'

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
    int x;
    int y;
} coord_t;

typedef struct {
    shape_t shape;
    coord_t *blks;
    int blk_cnt;
} tile_t;

typedef struct {
    char map[14][14];
} board_t;

tile_t *make_tile(shape_t shape);
int rot_tile(tile_t *tile, int theta);
int mir_tile(tile_t *tile);

#endif
