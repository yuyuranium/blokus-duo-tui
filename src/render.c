#include "blokus.h"
#include "render.h"
#include <ncurses.h>
#include <string.h>

const coord_t TILE_CURS[SHAPE_Z + 1] = {
    {.y = 15, .x = 70},  // SHAPE_O 
    {.y = 15, .x = 62},  // SHAPE_R 
    {.y = 15, .x = 55},  // SHAPE_M 
    {.y = 14, .x = 49},  // SHAPE_A 
    {.y = 2, .x = 37},   // SHAPE_E 
    {.y = 7, .x = 37},   // SHAPE_J 
    {.y = 6, .x = 42},   // SHAPE_S 
    {.y = 10, .x = 40},  // SHAPE_Q 
    {.y = 11, .x = 35},  // SHAPE_H 
    {.y = 11, .x = 69},  // SHAPE_F 
    {.y = 14, .x = 39},  // SHAPE_I 
    {.y = 2, .x = 50},   // SHAPE_L 
    {.y = 2, .x = 68},   // SHAPE_N 
    {.y = 6, .x = 46},   // SHAPE_P 
    {.y = 10, .x = 62},  // SHAPE_T 
    {.y = 11, .x = 48},  // SHAPE_U 
    {.y = 7, .x = 52},   // SHAPE_V 
    {.y = 11, .x = 56},  // SHAPE_W 
    {.y = 6, .x = 68},   // SHAPE_X 
    {.y = 2, .x = 58},   // SHAPE_Y 
    {.y = 6, .x = 61},   // SHAPE_Z 
};

const coord_t ALPHA_CURS[SHAPE_Z + 1] = {
    {.y = 14, .x = 70},  // SHAPE_O
    {.y = 14, .x = 63},  // SHAPE_D
    {.y = 14, .x = 55},  // SHAPE_M
    {.y = 13, .x = 48},  // SHAPE_A
    {.y = 0, .x = 38},   // SHAPE_E
    {.y = 4, .x = 36},   // SHAPE_J
    {.y = 4, .x = 41},   // SHAPE_S
    {.y = 9, .x = 41},   // SHAPE_Q
    {.y = 9, .x = 36},   // SHAPE_H
    {.y = 9, .x = 69},   // SHAPE_F
    {.y = 13, .x = 39},  // SHAPE_I
    {.y = 0, .x = 47},   // SHAPE_L
    {.y = 0, .x = 67},   // SHAPE_N
    {.y = 4, .x = 47},   // SHAPE_P
    {.y = 9, .x = 62},   // SHAPE_T
    {.y = 9, .x = 48},   // SHAPE_U
    {.y = 4, .x = 54},   // SHAPE_V
    {.y = 9, .x = 56},   // SHAPE_W
    {.y = 4, .x = 68},   // SHAPE_X
    {.y = 0, .x = 57},   // SHAPE_Y
    {.y = 4, .x = 61},   // SHAPE_Z
};

const shape_t tile_relation[4][6] = {
    {SHAPE_E, SHAPE_E, SHAPE_L, SHAPE_Y, SHAPE_Y, SHAPE_N},
    {SHAPE_J, SHAPE_S, SHAPE_P, SHAPE_V, SHAPE_Z, SHAPE_X},
    {SHAPE_H, SHAPE_Q, SHAPE_U, SHAPE_W, SHAPE_T, SHAPE_F},
    {SHAPE_I, SHAPE_I, SHAPE_A, SHAPE_R, SHAPE_D, SHAPE_M}
};

int init_all_colors() {
    start_color();
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
    return 0;
}

int render_board(gcb_t *gcb)
{
    mvprintw(11, 11, "+");
    mvprintw(6, 21, "+");
    mvprintw(1, 0, "   0 1 2 3 4 5 6 7 8 9 a b c d  │\n");
    for (int y = 0; y < N_ROW; ++y) {
        printw(" %x ", y);
        for (int x = 0; x < N_COL; ++x) {
            if (gcb->map[y][x] != -1) {
                printw("%s ", (gcb->map[y][x])? "□" : "■"); 
            } else {
                printw("%s ",
                       (((y == 4 && x == 9) || (y == 9 && x == 4)))? "+" : "·"); 
            }
        }
        printw(" │\n");
    }
    return 0;
}
    
int render_board_preview(rcb_t *rcb)
{
    gcb_t *gcb = rcb->gcb;
    tile_t *tile = rcb->chosen;
    int has_conflict = 0;
    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
        int blk_x = tile->pos.x + tile->blks[i].x;
        int blk_y = tile->pos.y + tile->blks[i].y;
        if (gcb->map[blk_y][blk_x] != -1) {  // has block on such pos
            has_conflict = 1; 
        }
    }
    
    start_color();
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
    if (has_conflict) {
        attron(COLOR_PAIR(RED_PAIR));
    } else {
        attron(COLOR_PAIR(GREEN_PAIR));
    }
    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
        int blk_x = tile->pos.x + tile->blks[i].x;
        int blk_y = tile->pos.y + tile->blks[i].y;
        mvprintw(blk_y + 2, blk_x * 2 + 3, "■");
    }
    if (has_conflict) {
        attroff(COLOR_PAIR(RED_PAIR));
    } else {
        attroff(COLOR_PAIR(GREEN_PAIR));
    }
    return 0;
}

int recover_board_preview(rcb_t *rcb)
{
    gcb_t *gcb = rcb->gcb;
    tile_t *tile = rcb->chosen;
    for (int i = 0; i < TILE[tile->shape].blk_cnt; ++i) {
        int blk_x = tile->pos.x + tile->blks[i].x;
        int blk_y = tile->pos.y + tile->blks[i].y;
        if (gcb->map[blk_y][blk_x] != -1) {
            mvprintw(blk_y + 2, blk_x * 2 + 3, "%s ",
                     (gcb->map[blk_y][blk_x])? "□" : "■"); 
        } else {
            mvprintw(blk_y + 2, blk_x * 2 + 3, "%s ",
                     (((blk_y == 4 && blk_x == 9) || 
                       (blk_y == 9 && blk_x == 4)))? "+" : "·"); 
        }
    }  
    return 0;
}

int render_tiles(gcb_t *gcb, int player)
{
    for (int i = 0; i < SHAPE_Z + 1; ++i) {
        if (gcb->hand[player][i]) {
            mvprintw(ALPHA_CURS[i].y, ALPHA_CURS[i].x, "%c", TILE[i].alpha);
            for (int j = 0; j < TILE[i].blk_cnt; ++j) {
                mvprintw(TILE_CURS[i].y + TILE[i].blks[j].y, 
                         TILE_CURS[i].x + TILE[i].blks[j].x * 2,
                         "%s", (player)? "□" : "■");
            }
        } else {
            mvprintw(ALPHA_CURS[i].y, ALPHA_CURS[i].x, " ");
            for (int j = 0; j < TILE[i].blk_cnt; ++j) {
                mvprintw(TILE_CURS[i].y + TILE[i].blks[j].y, 
                         TILE_CURS[i].x + TILE[i].blks[j].x * 2,
                         " ");
            }
        }
    }
    return 0; 
}

int render_tile_preview(gcb_t *gcb, shape_t shape)
{
    start_color();
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
    if (!gcb->hand[0][shape]) {
        return -1;
    }
    attron(COLOR_PAIR(YELLOW_PAIR));
    mvprintw(ALPHA_CURS[shape].y, ALPHA_CURS[shape].x, "%c", TILE[shape].alpha);
    for (int j = 0; j < TILE[shape].blk_cnt; ++j) {
        mvprintw(TILE_CURS[shape].y + TILE[shape].blks[j].y, 
                 TILE_CURS[shape].x + TILE[shape].blks[j].x * 2,
                 "■");
    }
    attroff(COLOR_PAIR(YELLOW_PAIR));
    return 0;
}

int recover_tile_preview(gcb_t *gcb, shape_t shape)
{
    if (!gcb->hand[0][shape]) {
        return -1;
    }
    mvprintw(ALPHA_CURS[shape].y, ALPHA_CURS[shape].x, "%c", TILE[shape].alpha);
    for (int j = 0; j < TILE[shape].blk_cnt; ++j) {
        mvprintw(TILE_CURS[shape].y + TILE[shape].blks[j].y, 
                 TILE_CURS[shape].x + TILE[shape].blks[j].x * 2,
                 "■");
    }
    return 0;
}

int render_message_log(char *msg[7], int *color[7])
{
    start_color();
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);
    
    mvprintw(17, 0, "╔══════════════════════════════════════════════════════╗");
    for (int i = 0; i < 7; ++i) {
        mvprintw(i + 18, 0, "║ ");
        attron(COLOR_PAIR(*color[i]));
        mvprintw(i + 18, 2, "%s", msg[i]);
        for (int j = 0; j < MAX_LOG_LEN - strlen(msg[i]); ++j) {
            printw(" ");
        } 
        attroff(COLOR_PAIR(*color[i]));
        printw("║");
    }
    mvprintw(25, 0, "╚══════════════════════════════════════════════════════╝");
    return 0;
}

int render_score_board()
{
    mvprintw(17, 57, "╔══════════════╗");
    mvprintw(18, 57, "║ >          < ║");
    mvprintw(19, 57, "╚══════════════╝");
    mvprintw(20, 57, "╔══════════════╗");
    mvprintw(21, 57, "║ Score Board  ║");
    for (int i = 22; i < 25; ++i) {
        mvprintw(i, 57, "║              ║");
    }
    mvprintw(25, 57, "╚══════════════╝");
    return 0;
}

int render_score(rcb_t *rcb, int first_in) {
    gcb_t *gcb = rcb->gcb;
    if (!gcb->turn && first_in) {
        attron(A_BLINK);
        mvprintw(18, 61, "Player %d", gcb->turn);
        attroff(A_BLINK);
    } else {
        mvprintw(18, 61, "Player %d", gcb->turn);
    }
    if (gcb->score[0] > gcb->score[1]) {
        mvprintw(23, 59, "Player 0: %2d", gcb->score[0]);
        mvprintw(24, 59, "Player 1: %2d", gcb->score[1]);
    } else {
        mvprintw(23, 59, "Player 1: %2d", gcb->score[1]);
        mvprintw(24, 59, "Player 0: %2d", gcb->score[0]);
    }
    return 0;
}

int shift_msg(char *msg[7], int *color[7])
{
    for (int i = 0; i < 6; ++i) {
        strncpy(msg[i], msg[i + 1], MAX_LOG_LEN);
        msg[i][strlen(msg[i + 1])] = '\0';
        *color[i] = *color[i + 1];
    }
    *color[6] = 0;
    return 0;
}
