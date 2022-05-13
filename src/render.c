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

int render_board(gcb_t *gcb)
{
    mvprintw(1, 0, "   0 1 2 3 4 5 6 7 8 9 a b c d  │\n");
    for (int y = 0; y < N_ROW; ++y) {
        printw(" %x ", y);
        for (int x = 0; x < N_COL; ++x) {
            printw("%s ", (gcb->map[y][x] == -1)? "·" :
                          (gcb->map[y][x] == 0)? "□" : "■"); 
        }
        printw(" │\n");
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

int render_chosen_tile(tile_t *tile)
{
    mvprintw(17, 0, " ╭─────────────────╮");
    for (int i = 18; i <= 24; ++i) {
        mvprintw(i, 0, " │                 │");
    }
    mvprintw(25, 0, " ╰─────────────────╯");

    start_color();
    init_pair(CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(CYAN_PAIR));
    mvprintw(21, 10, "■");
    attroff(COLOR_PAIR(CYAN_PAIR));
    
    for (int i = 1; i < TILE[tile->shape].blk_cnt; ++i) {
        int y = 21 + tile->blks[i].y;
        int x = 10 + tile->blks[i].x * 2;
        mvprintw(y, x, "■");
    }
    return 0;
}

int render_message_log(char *msg[6], int len[6])
{
    mvprintw(17, 23, "╔══════════════════════════════════════════════════════╗");
    for (int i = 0; i < 6; ++i) {
        mvprintw(i + 18, 23, "║ %s", msg[i]);
        for (int j = 0; j < 53 - len[i]; ++j) {
            printw(" ");
        } 
        printw("║");
    }
    mvprintw(24, 23, "╚══════════════════════════════════════════════════════╝");
    mvprintw(25, 23, ">                                                      ");
    move(25, 25);
    return 0;
}

int shift_msg(char *msg[6], int len[6])
{
    for (int i = 0; i < 5; ++i) {
        strncpy(msg[i], msg[i + 1], len[i + 1]);
        len[i] = len[i + 1];
        msg[i][len[i]] = '\0';
    }
    return 0;
}
