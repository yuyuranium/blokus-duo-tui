#include "client.h"
#include "blokus.h"
#include <ncurses.h>
#include <stdlib.h>

int choose_tile_handler(int c, rcb_t *rcb, coord_t *coord)
{
    if (rcb->render_player == 0) 
        recover_tile_preview(rcb->gcb, tile_relation[coord->y][coord->x]);
    switch (c) {
        case 'h':
        case 'H':
        case KEY_LEFT:
            if (coord->x > 0) {
                coord->x--;
                if (coord->x > 0 
                    && tile_relation[coord->y][coord->x] 
                    == tile_relation[coord->y][coord->x+1]) coord->x--;
            }
            break;
        case 'j':
        case 'J':
        case KEY_DOWN:
            if (coord->y < 3) coord->y++;
            break;
        case 'k':
        case 'K':
        case KEY_UP:
            if (coord->y > 0) coord->y--;
            break;
        case 'l':
        case 'L':
        case KEY_RIGHT:
            if (coord->x < 5) {
                coord->x++;
                if (coord->x < 5 
                    && tile_relation[coord->y][coord->x] 
                    == tile_relation[coord->y][coord->x-1]) coord->x++;
            }
            break;
        case 'c':
        case 'C':
            rcb->render_player = !rcb->render_player;
            render_tiles(rcb->gcb, rcb->render_player);
            break;
        case ' ': {
            tile_t *chosen = sel_tile(rcb->gcb, tile_relation[coord->y][coord->x]);
            chosen->pos.x = 7;
            chosen->pos.y = 7;
            rcb->chosen = chosen;
            rcb->state++;
            render_board_preview(rcb);
            break;
        }
    }
    if (rcb->render_player == 0) 
        render_tile_preview(rcb->gcb, tile_relation[coord->y][coord->x]);
    return 0;
}

int positioning_handler(int c, rcb_t *rcb)
{
    recover_board_preview(rcb);
    tile_t* chosen = rcb->chosen;
    coord_t tmp_pos = chosen->pos;
    int operation = 0;
    switch (c) {
        case 'h':
        case 'H':
        case KEY_LEFT:
            chosen->pos.x--;
            break;
        case 'j':
        case 'J':
        case KEY_DOWN:
            chosen->pos.y++;
            break;
        case 'k':
        case 'K':
        case KEY_UP:
            chosen->pos.y--;
            break;
        case 'l':
        case 'L':
        case KEY_RIGHT:
            chosen->pos.x++;
            break;
        case 'c':
        case 'C':
            rcb->render_player = !rcb->render_player;
            render_tiles(rcb->gcb, rcb->render_player);
            break;
        case 'r':
            operation = 1; 
            rot_tile(chosen, 90); 
            break;
        case 'R':
            operation = 2; 
            rot_tile(chosen, 270); 
            break;
        case 'm':
        case 'M':
            operation = 3; 
            mir_tile(chosen);
            break;
        case KEY_BACKSPACE:
        case 'q':
        case 'Q':
            recover_board_preview(rcb); 
            rcb->state--;
            return 0;
        case ' ': {
             rcb->state++;
             break;
        }
    }
    for (int i = 0; i < TILE[chosen->shape].blk_cnt; ++i) {
        if (chosen->pos.x + chosen->blks[i].x >= N_COL
         || chosen->pos.x + chosen->blks[i].x < 0 
         || chosen->pos.y + chosen->blks[i].y >= N_ROW
         || chosen->pos.y + chosen->blks[i].y < 0) {
            chosen->pos = tmp_pos;
            if (operation) {
                switch (operation) {
                    case 1:
                        rot_tile(chosen, 270);
                        break;
                    case 2:
                        rot_tile(chosen, 270);
                        break;
                    case 3:
                        mir_tile(chosen);
                        break;
                }
            }
            break;
        }
    }
    render_board_preview(rcb);
    return 0;
}
