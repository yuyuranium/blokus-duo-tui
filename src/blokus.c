#include "blokus.h"
#include <stdlib.h>

gcb_t *init_gcb()
{
    gcb_t *gcb = malloc(sizeof(gcb_t));
    gcb->turn = 0;
    gcb->score[0] = 0;
    gcb->score[1] = 0;
    for (int i = 0; i <= SHAPE_Z; ++i) {
        gcb->hand[0][i] = 1;
        gcb->hand[1][i] = 1;
    }
    for (int y = 0; y < N_ROW; ++y) {
        for (int x = 0; x < N_COL; ++x) {
            gcb->map[y][x] = -1;
        }
    }
    return gcb;
}

int test_place(gcb_t *gcb, tile_t *tile, coord_t coord)
{
    int p = gcb->turn, valid = 0;

    // If the player hasn't place any tiles yet
    if (gcb->score[p] == 0) {
        for (int i = 0; i < tile->blk_cnt; ++i) {
            int y = coord.y + tile->blks[i].y, x = coord.x + tile->blks[i].x;

            // Out of map
            if (!within_map(y, x))
                return -1;

            // Check if covering starting point
            if (y == STARTING_POINT[p].y && x == STARTING_POINT[p].x)
                valid = 1;
        }
    } else {
        for (int i = 0; i < tile->blk_cnt; ++i) {
            int y = coord.y + tile->blks[i].y, x = coord.x + tile->blks[i].x;

            // Check if within map
            if (within_map(y, x)) {
                // Overlapping
                if (gcb->map[y][x] != -1)
                    return -1;

                for (int j = 0; j < 4; ++j) {
                    int ey = y + EDGE[i].y, ex = x + EDGE[i].x;

                    // Edge-to-edge contact is not allowed
                    if (within_map(ey, ex) && gcb->map[ey][ex] == p)
                        return -1;
                }

                for (int j = 0; j < 4; ++j) {
                    int cy = y + CORNER[i].y, cx = x + CORNER[i].x;

                    // Must have at least one corner-to-corner contact
                    if (within_map(cy, cx) && gcb->map[cy][cx] == p)
                        valid = 1;
                }
            } else {
                return -1;
            }
        }
    }

    return (valid)? 0 : -1;
}

tile_t *make_tile(shape_t shape)
{
    tile_t *tile = malloc(sizeof(tile_t));
    int blk_cnt, can_mir = 0, rot_cnt = 0;
    coord_t *blks;

    if (shape <= SHAPE_O) {
        // monomino
        blk_cnt = 1;
        blks = malloc(blk_cnt * sizeof(coord_t));
        blks[0].y = 0; blks[0].x = 0;
    } else if (shape <= SHAPE_R) {
        // domino
        blk_cnt = 2;
        blks = malloc(blk_cnt * sizeof(coord_t));
        blks[0].y = 0; blks[0].x = 0;
        blks[1].y = 0; blks[1].x = 1;
        rot_cnt = 1;
    } else if (shape <= SHAPE_A) {
        // trominoes
        blk_cnt = 3;
        blks = malloc(blk_cnt * sizeof(coord_t));
        blks[0].y = 0; blks[0].x = 0;
        switch (shape) {
        case SHAPE_M:
            blks[1].y = 0; blks[1].x = -1;
            blks[2].y = 0; blks[2].x = 1;
            rot_cnt = 1;
            break;
        case SHAPE_A:
            blks[1].y = 0; blks[1].x = -1;
            blks[2].y = 1; blks[2].x = 0;
            rot_cnt = 3;
            break;
        default:
            ;
        }
    } else if (shape <= SHAPE_H) {
        // tetrominoes
        blk_cnt = 4;
        blks = malloc(blk_cnt * sizeof(coord_t));
        blks[0].y = 0; blks[0].x = 0;
        switch (shape) {
        case SHAPE_E:
            blks[1].y = 0; blks[1].x = -1;
            blks[2].y = 0; blks[2].x = 1;
            blks[3].y = 0; blks[3].x = 2;
            rot_cnt = 1;
            break;
        case SHAPE_J:
            blks[1].y = -2; blks[1].x = 0;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 0; blks[3].x = -1;
            rot_cnt = 3;
            can_mir = 1;
            break;
        case SHAPE_S:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = 0; blks[2].x = -1;
            blks[3].y = 1; blks[3].x = 0;
            rot_cnt = 1;
            can_mir = 1;
            break;
        case SHAPE_Q:
            blks[1].y = 0; blks[1].x = 1;
            blks[2].y = 1; blks[2].x = 0;
            blks[3].y = 1; blks[3].x = 1;
            break;
        case SHAPE_H:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = 0; blks[2].x = 1;
            blks[3].y = 1; blks[3].x = 0;
            rot_cnt = 3;
            break;
        default:
            ;
        }
    } else if (shape <= SHAPE_Z) {
        // pentominoes
        blk_cnt = 5;
        blks = malloc(blk_cnt * sizeof(coord_t));
        blks[0].y = 0; blks[0].x = 0;
        switch (shape) {
        case SHAPE_F:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 1; blks[4].x = 0;
            rot_cnt = 3;
            can_mir = 1;
            break;
        case SHAPE_I:
            blks[1].y = 0; blks[1].x = -2;
            blks[2].y = 0; blks[2].x = -1;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 0; blks[4].x = 2;
            rot_cnt = 1;
            break;
        case SHAPE_L:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = 0; blks[2].x = -3;
            blks[3].y = 0; blks[3].x = -2;
            blks[4].y = 0; blks[4].x = -1;
            rot_cnt = 3;
            can_mir = 1;
            break;
        case SHAPE_N:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = -1; blks[2].x = 1;
            blks[3].y = 0; blks[3].x = -2;
            blks[4].y = 0; blks[4].x = -1;
            rot_cnt = 3;
            can_mir = 1;
            break;
        case SHAPE_P:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = -1; blks[2].x = 1;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 1; blks[4].x = 0;
            rot_cnt = 3;
            can_mir = 1;
            break;
        case SHAPE_T:
            blks[1].y = 0; blks[1].x = -1;
            blks[2].y = 0; blks[2].x = 1;
            blks[3].y = 1; blks[3].x = 0;
            blks[4].y = 2; blks[4].x = 0;
            rot_cnt = 3;
            break;
        case SHAPE_U:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = -1; blks[2].x = 1;
            blks[3].y = 0; blks[3].x = -1;
            blks[4].y = 0; blks[4].x = 1;
            rot_cnt = 3;
            break;
        case SHAPE_V:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 1; blks[4].x = 0;
            rot_cnt = 3;
            break;
        case SHAPE_W:
            blks[1].y = -2; blks[1].x = 0;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 0; blks[4].x = 2;
            rot_cnt = 3;
            break;
        case SHAPE_X:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = 0; blks[2].x = -1;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 1; blks[4].x = 0;
            break;
        case SHAPE_Y:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = 0; blks[2].x = -2;
            blks[3].y = 0; blks[3].x = -1;
            blks[4].y = 0; blks[4].x = 1;
            rot_cnt = 3;
            can_mir = 1;
            break;
        case SHAPE_Z:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 1; blks[3].x = 0;
            blks[4].y = 1; blks[4].x = 1;
            rot_cnt = 1;
            can_mir = 1;
            break;
        default:
            ;
        }
    } else {
        free(tile);
        return NULL;
    }

    tile->blk_cnt = blk_cnt;
    tile->blks = blks;
    tile->can_mir = can_mir;
    tile->rot_cnt = rot_cnt;
    return tile;
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

    for (int i = 0; i < tile->blk_cnt; ++i) {
        int x = tile->blks[i].x, y = tile->blks[i].y;
        tile->blks[i].y = rot_mtx[1][0] * x + rot_mtx[1][1] * y;
        tile->blks[i].x = rot_mtx[0][0] * x + rot_mtx[0][1] * y;
    }
    return 0;
}

int mir_tile(tile_t *tile)
{
    for (int i = 0; i < tile->blk_cnt; ++i)
        tile->blks[i].x *= -1;
    return 0;
}
