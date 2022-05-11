#include "blokus.h"
#include <stdlib.h>

tile_t *make_tile(shape_t shape)
{
    tile_t *tile = malloc(sizeof(tile_t));
    int blk_cnt;
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
    } else if (shape <= SHAPE_A) {
        // trominoes
        blk_cnt = 3;
        blks = malloc(blk_cnt * sizeof(coord_t));
        blks[0].y = 0; blks[0].x = 0;
        switch (shape) {
        case SHAPE_M:
            blks[1].y = 0; blks[1].x = -1;
            blks[2].y = 0; blks[2].x = 1;
            break;
        case SHAPE_A:
            blks[1].y = 0; blks[1].x = -1;
            blks[2].y = 1; blks[2].x = 0;
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
            break;
        case SHAPE_J:
            blks[1].y = -2; blks[1].x = 0;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 0; blks[3].x = -1;
            break;
        case SHAPE_S:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = 0; blks[2].x = -1;
            blks[3].y = 1; blks[3].x = 0;
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
            break;
        case SHAPE_I:
            blks[1].y = 0; blks[1].x = -2;
            blks[2].y = 0; blks[2].x = -1;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 0; blks[4].x = 2;
            break;
        case SHAPE_L:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = 0; blks[2].x = -3;
            blks[3].y = 0; blks[3].x = -2;
            blks[4].y = 0; blks[4].x = -1;
            break;
        case SHAPE_N:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = -1; blks[2].x = 1;
            blks[3].y = 0; blks[3].x = -2;
            blks[4].y = 0; blks[4].x = -1;
            break;
        case SHAPE_P:
            blks[1].y = -1; blks[1].x = 0;
            blks[2].y = -1; blks[2].x = 1;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 1; blks[4].x = 0;
            break;
        case SHAPE_T:
            blks[1].y = 0; blks[1].x = -1;
            blks[2].y = 0; blks[2].x = 1;
            blks[3].y = 1; blks[3].x = 0;
            blks[4].y = 2; blks[4].x = 0;
            break;
        case SHAPE_U:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = -1; blks[2].x = 1;
            blks[3].y = 0; blks[3].x = -1;
            blks[4].y = 0; blks[4].x = 1;
            break;
        case SHAPE_V:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 1; blks[4].x = 0;
            break;
        case SHAPE_W:
            blks[1].y = -2; blks[1].x = 0;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 0; blks[3].x = 1;
            blks[4].y = 0; blks[4].x = 2;
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
            break;
        case SHAPE_Z:
            blks[1].y = -1; blks[1].x = -1;
            blks[2].y = -1; blks[2].x = 0;
            blks[3].y = 1; blks[3].x = 0;
            blks[4].y = 1; blks[4].x = 1;
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
        tile->blks[i].x = rot_mtx[0][0] * x + rot_mtx[0][1] * y;
        tile->blks[i].y = rot_mtx[1][0] * x + rot_mtx[1][1] * y;
    }
    return 0;
}

int mir_tile(tile_t *tile)
{
    for (int i = 0; i < tile->blk_cnt; ++i)
        tile->blks[i].x *= -1;
    return 0;
}
