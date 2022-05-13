#ifndef _BLOKUS_H
#define _BLOKUS_H 

#define N_ROW 14
#define N_COL 14

#define within_map(y, x) ((y) >= 0 && (y) < N_ROW && (x) >= 0 && (x) < N_COL)

typedef enum {
    // monomino
    SHAPE_M,
    // domino
    SHAPE_D,
    // trominoes
    SHAPE_R,
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

struct _tile_attr {
    char alpha;
    coord_t blks[5];
    int blk_cnt;
    int can_mir;
    int rot_cnt;
};

typedef struct {
    shape_t shape;
    coord_t *blks;
    coord_t pos;
} tile_t;

typedef struct {
    int turn;
    shape_t sel_shape;
    tile_t *hand[2][SHAPE_Z + 1];
    int score[2];
    int next_empty[N_ROW * N_COL];
    int prev_empty[N_ROW * N_COL];
    int map[N_ROW][N_COL];
} gcb_t;  // game control block

extern const coord_t STARTING_POINT[2];
extern const coord_t CORNER[4];
extern const coord_t EDGE[4];
extern const struct _tile_attr TILE[SHAPE_Z + 1];

gcb_t *init_gcb(int turn);
int can_place(gcb_t *gcb);  // TODO implement this
tile_t *sel_tile(gcb_t *gcb, int shape);
int update(gcb_t *gcb, unsigned char *code);
int test_place(gcb_t *gcb, tile_t *tile);  // will be static

tile_t *make_tile(shape_t shape);  // will be static
int rot_tile(tile_t *tile, int theta);
int mir_tile(tile_t *tile);
int encode_tile(tile_t *tile, unsigned char *code_out);
tile_t *decode_tile(unsigned char *code);  // will be static

#endif
