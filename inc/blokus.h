#ifndef _BLOKUS_H
#define _BLOKUS_H 

#define N_ROW 14
#define N_COL 14

#define OK 0
#define EOG_P 1    // end-of-game: player1 wins
#define EOG_Q 2    // end-of-game: player2 wins
#define EOG_T 3    // end-of-game: tie
#define INV_N -1   // invalid: tile not present
#define INV_S -2   // invalid: tile not selected yet
#define INV_P -3   // invalid: not covering starting point placement
#define INV_E -4   // invalid: edge-to-edge contact placement
#define INV_C -5   // invalid: no corner-to-corner contact placement
#define INV_L -6   // invalid: overlapping placement
#define INV_O -7   // invalid: out-of-map placement
#define ERR_V -8   // internal error: value error
#define ERR_E -9   // internal error: encode tile failed
#define ERR_D -10  // internal error: decode tile failed

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
    shape_t sel;
    tile_t *hand[2][SHAPE_Z + 1];
    int score[2];
    // the linked list that link all the empty positions on the map
    int empty;
    int next_empty[N_ROW * N_COL];
    int prev_empty[N_ROW * N_COL];
    int map[N_ROW][N_COL];
    char code[16];
    int status;
} gcb_t;  // game control block

extern const coord_t STARTING_POINT[2];
extern const coord_t CORNER[4];
extern const coord_t EDGE[4];
extern const struct _tile_attr TILE[SHAPE_Z + 1];

// Functions driving the blockus game
gcb_t *init_gcb(int first);
tile_t *sel_tile(gcb_t *gcb, int shape);
int is_valid(gcb_t *gcb);
int update(gcb_t *gcb, char *code);

// Functions manipulating tiles
int rot_tile(tile_t *tile, int theta);
int mir_tile(tile_t *tile);

#endif
