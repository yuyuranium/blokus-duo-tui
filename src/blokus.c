#include "blokus.h"
#include <stdlib.h>
#if DEBUG
#include <stdio.h>
#include <string.h>
#endif

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

const struct _tile_attr TILE[SHAPE_Z + 1] = {
    {   // SHAPE_M
        .alpha = 'M',
        .blks = {
            {.y = 0, .x = 0},
        },
        .blk_cnt = 1,
        .can_mir = 0,
        .rot_cnt = 0
    },
    {   // SHAPE_D
        .alpha = 'D',
        .blks = {
            {.y = 0, .x = 0},
            {.y = 0, .x = 1},
        },
        .blk_cnt = 2,
        .can_mir = 0,
        .rot_cnt = 1
    },
    {   // SHAPE_R
        .alpha = 'R',
        .blks = {
            {.y = 0, .x = 0},
            {.y = 0, .x = -1},
            {.y = 0, .x = 1},
        },
        .blk_cnt = 3,
        .can_mir = 0,
        .rot_cnt = 1
    },
    {   // SHAPE_A
        .alpha = 'A',
        .blks = {
            {.y = 0, .x = 0},
            {.y = 0, .x = -1},
            {.y = 1, .x = 0},
        },
        .blk_cnt = 3,
        .can_mir = 0,
        .rot_cnt = 3
    },
    {   // SHAPE_E
        .alpha = 'E',
        .blks = {
            {.y = 0, .x = 0},
            {.y = 0, .x = -1},
            {.y = 0, .x = 1},
            {.y = 0, .x = 2},
        },
        .blk_cnt = 4,
        .can_mir = 0,
        .rot_cnt = 1
    },
    {   // SHAPE_J
        .alpha = 'J',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -2, .x = 0},
            {.y = -1, .x = 0},
            {.y = 0, .x = -1},
        },
        .blk_cnt = 4,
        .can_mir = 1,
        .rot_cnt = 3
    },
    {   // SHAPE_S
        .alpha = 'S',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = -1},
            {.y = 0, .x = -1},
            {.y = 1, .x = 0},
        },
        .blk_cnt = 4,
        .can_mir = 1,
        .rot_cnt = 1
    },
    {   // SHAPE_Q
        .alpha = 'Q',
        .blks = {
            {.y = 0, .x = 0},
            {.y = 0, .x = 1},
            {.y = 1, .x = 0},
            {.y = 1, .x = 1},
        },
        .blk_cnt = 4,
        .can_mir = 0,
        .rot_cnt = 0
    },
    {   // SHAPE_H
        .alpha = 'H',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = 0},
            {.y = 0, .x = 1},
            {.y = 1, .x = 0},
        },
        .blk_cnt = 4,
        .can_mir = 0,
        .rot_cnt = 3
    },
    {   // SHAPE_F
        .alpha = 'F',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = -1},
            {.y = -1, .x = 0},
            {.y = 0, .x = 1},
            {.y = 1, .x = 0},
        },
        .blk_cnt = 5,
        .can_mir = 1,
        .rot_cnt = 3
    },
    {   // SHAPE_I
        .alpha = 'I',
        .blks = {
            {.y = 0, .x = 0},
            {.y = 0, .x = -2},
            {.y = 0, .x = -1},
            {.y = 0, .x = 1},
            {.y = 0, .x = 2},
        },
        .blk_cnt = 5,
        .can_mir = 0,
        .rot_cnt = 1
    },
    {   // SHAPE_L
        .alpha = 'L',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = 0},
            {.y = 0, .x = -3},
            {.y = 0, .x = -2},
            {.y = 0, .x = -1},
        },
        .blk_cnt = 5,
        .can_mir = 1,
        .rot_cnt = 3
    },
    {   // SHAPE_N
        .alpha = 'N',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = 0},
            {.y = -1, .x = 1},
            {.y = 0, .x = -2},
            {.y = 0, .x = -1},
        },
        .blk_cnt = 5,
        .can_mir = 1,
        .rot_cnt = 3
    },
    {   // SHAPE_P
        .alpha = 'P',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = 0},
            {.y = -1, .x = 1},
            {.y = 0, .x = 1},
            {.y = 1, .x = 0},
        },
        .blk_cnt = 5,
        .can_mir = 1,
        .rot_cnt = 3
    },
    {   // SHAPE_T
        .alpha = 'T',
        .blks = {
            {.y = 0, .x = 0},
            {.y = 0, .x = -1},
            {.y = 0, .x = 1},
            {.y = 1, .x = 0},
            {.y = 2, .x = 0},
        },
        .blk_cnt = 5,
        .can_mir = 0,
        .rot_cnt = 3
    },
    {   // SHAPE_U
        .alpha = 'U',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = -1},
            {.y = -1, .x = 1},
            {.y = 0, .x = -1},
            {.y = 0, .x = 1},
        },
        .blk_cnt = 5,
        .can_mir = 0,
        .rot_cnt = 3
    },
    {   // SHAPE_V
        .alpha = 'V',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -2, .x = 0},
            {.y = -1, .x = 0},
            {.y = 0, .x = 1},
            {.y = 0, .x = 2},
        },
        .blk_cnt = 5,
        .can_mir = 0,
        .rot_cnt = 3
    },
    {   // SHAPE_W
        .alpha = 'W',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = -1},
            {.y = 0, .x = -1},
            {.y = 1, .x = 0},
            {.y = 1, .x = 1},
        },
        .blk_cnt = 5,
        .can_mir = 0,
        .rot_cnt = 3
    },
    {   // SHAPE_X
        .alpha = 'X',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = 0},
            {.y = 0, .x = -1},
            {.y = 0, .x = 1},
            {.y = 1, .x = 0},
        },
        .blk_cnt = 5,
        .can_mir = 0,
        .rot_cnt = 0
    },
    {   // SHAPE_Y
        .alpha = 'Y',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = 0},
            {.y = 0, .x = -2},
            {.y = 0, .x = -1},
            {.y = 0, .x = 1},
        },
        .blk_cnt = 5,
        .can_mir = 1,
        .rot_cnt = 3
    },
    {   // SHAPE_Z
        .alpha = 'Z',
        .blks = {
            {.y = 0, .x = 0},
            {.y = -1, .x = -1},
            {.y = -1, .x = 0},
            {.y = 1, .x = 0},
            {.y = 1, .x = 1},
        },
        .blk_cnt = 5,
        .can_mir = 1,
        .rot_cnt = 1
    },
};

static tile_t *make_tile(shape_t shape)
{
    if (shape < SHAPE_M || shape > SHAPE_Z)
        return NULL;

    tile_t *tile = malloc(sizeof(tile_t));
    tile->shape = shape;
    tile->blks = malloc(TILE[shape].blk_cnt * sizeof(coord_t));
    for (int i = 0; i < TILE[shape].blk_cnt; ++i) {
        tile->blks[i].y = TILE[shape].blks[i].y;
        tile->blks[i].x = TILE[shape].blks[i].x;
    }
    tile->pos.y = 0; tile->pos.x = 0;
    return tile;
}

static int encode_tile(char *code, tile_t *tile)
{
    if (!tile || !code)
        return -1;

    char *c = &code[0];
    rot_tile(tile, 180);
    *c++ = tile->shape & 0xff;
    *c++ = N_ROW - tile->pos.y - 1; *c++ = N_COL - tile->pos.x - 1;
    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
        *c++ = tile->blks[i].y & 0xff;
        *c++ = tile->blks[i].x & 0xff;
    }
    *c = 0;

#if DEBUG
    printf("code: ");
    char *p = c;
    for (c = &code[0]; c != p; ++c) {
        printf("(%x)", (int) *c);
    }
    printf("\n");
#endif
    return 0;
}

static tile_t *decode_tile(char *code)
{
    char *c = &code[0];
    if (*c < SHAPE_M || *c > SHAPE_Z)
        return NULL;

    tile_t *tile = malloc(sizeof(tile_t));
    tile->shape = (int) *c++;
    tile->pos.y = (int) *c++; tile->pos.x = (int) *c++;
    tile->blks = malloc(TILE[tile->shape].blk_cnt * sizeof(coord_t));
    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
        tile->blks[i].y = (int) *c++;
        tile->blks[i].x = (int) *c++;
    }

#if DEBUG
    char map[8][8];
    memset(map, '.', 64);
    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i)
        map[4 + tile->blks[i].y][4 + tile->blks[i].x] = 'o';
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            printf("%c ", map[i][j]);
        printf("\n");
    }
#endif
    return tile;
}

static int test_place(gcb_t *gcb, tile_t *tile)
{
    int p = gcb->turn, valid = 0;
    coord_t pos = tile->pos;

    // If the player hasn't place any tiles yet
    if (gcb->score[p] == 0) {
        for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
            int y = pos.y + tile->blks[i].y, x = pos.x + tile->blks[i].x;

            // Out of map
            if (!within_map(y, x))
                return -1;

            // Check if covering starting point
            if (y == STARTING_POINT[p].y && x == STARTING_POINT[p].x)
                valid = 1;
        }
    } else {
        for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
            int y = pos.y + tile->blks[i].y, x = pos.x + tile->blks[i].x;

            // Out of map or overlapping
            if (!within_map(y, x) || gcb->map[y][x] != -1)
                return -1;

            for (int j = 0; j < 4; ++j) {
                int ey = y + EDGE[j].y, ex = x + EDGE[j].x;

                // Edge-to-edge contact is not allowed
                if (within_map(ey, ex) && gcb->map[ey][ex] == p)
                    return -1;
            }

            for (int j = 0; j < 4; ++j) {
                int cy = y + CORNER[j].y, cx = x + CORNER[j].x;

                // Must have at least one corner-to-corner contact
                if (within_map(cy, cx) && gcb->map[cy][cx] == p)
                    valid = 1;
            }
        }
    }

    return (valid)? 0 : -1;
}

gcb_t *init_gcb(int turn)
{
    gcb_t *gcb = malloc(sizeof(gcb_t));
    gcb->turn = turn;
    gcb->sel_shape = -1;
    gcb->score[0] = 0;
    gcb->score[1] = 0;
    for (int s = 0; s <= SHAPE_Z; ++s) {
        gcb->hand[0][s] = make_tile(s);
        gcb->hand[1][s] = make_tile(s);
    }
    for (int y = 0; y < N_ROW; ++y) {
        for (int x = 0; x < N_COL; ++x) {
            gcb->map[y][x] = -1;
        }
    }
    for (int i = 0; i < 195; ++i)
        gcb->next_empty[i] = i + 1;
    gcb->next_empty[195] = -1;
    for (int i = 1; i < 196; ++i)
        gcb->prev_empty[i] = i - 1;
    gcb->prev_empty[0] = -1;
    return gcb;
}

tile_t *sel_tile(gcb_t *gcb, int shape)
{
    gcb->sel_shape = shape;
    return gcb->hand[gcb->turn][gcb->sel_shape];
}

int can_place(gcb_t *gcb)
{
    int p = gcb->turn;
    tile_t *tile;
    if (gcb->sel_shape == -1)
        return -1;

    tile = gcb->hand[p][gcb->sel_shape];
    return test_place(gcb, tile) == 0;
}

int update(gcb_t *gcb, char *code)
{
    int p = gcb->turn;
    tile_t *tile = code ? decode_tile(code) : gcb->hand[p][gcb->sel_shape];

    if (test_place(gcb, tile) < 0)
        return -1;  // reject invalid update

    coord_t pos = tile->pos;
    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
        int y = pos.y + tile->blks[i].y, x = pos.x + tile->blks[i].x;
        int k = N_ROW * y + x;
        gcb->map[y][x] = p;
        gcb->next_empty[gcb->prev_empty[k]] = gcb->next_empty[k];
        gcb->prev_empty[gcb->next_empty[k]] = gcb->prev_empty[k];
    }

    encode_tile(gcb->code, tile);  // record the last update with gcb->code
    gcb->score[p] += TILE[tile->shape].blk_cnt;
    gcb->turn = !gcb->turn;
    gcb->sel_shape = -1;
    free(tile);
    return 0;
}

int rot_tile(tile_t *tile, int theta)
{
    int rot_mtx[2][2];

    if ((theta %= 360) < 0)
        theta += 360;

    switch (theta) {
    case 90:
        rot_mtx[0][0] = 0;
        rot_mtx[0][1] = -1;
        rot_mtx[1][0] = 1;
        rot_mtx[1][1] = 0;
        break;
    case 180:
        rot_mtx[0][0] = -1;
        rot_mtx[0][1] = 0;
        rot_mtx[1][0] = 0;
        rot_mtx[1][1] = -1;
        break;
    case 270:
        rot_mtx[0][0] = 0;
        rot_mtx[0][1] = 1;
        rot_mtx[1][0] = -1;
        rot_mtx[1][1] = 0;
        break;
    default:
        return -1;
    }

    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
        int x = tile->blks[i].x, y = tile->blks[i].y;
        tile->blks[i].y = rot_mtx[1][0] * x + rot_mtx[1][1] * y;
        tile->blks[i].x = rot_mtx[0][0] * x + rot_mtx[0][1] * y;
    }
    return 0;
}

int mir_tile(tile_t *tile)
{
    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i)
        tile->blks[i].x *= -1;
    return 0;
}
