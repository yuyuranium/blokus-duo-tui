#include "blokus.h"
#include "render.h"
#include <ncurses.h>

const coord_t TILE_CURS[SHAPE_Z + 1] = {
     // SHAPE_O 
     {.y = 15, .x = 68},
     // SHAPE_R 
     {.y = 15, .x = 60},
     // SHAPE_M 
     {.y = 15, .x = 53},
     // SHAPE_A 
     {.y = 14, .x = 47},
     // SHAPE_E 
     {.y = 2, .x = 35},
     // SHAPE_J 
     {.y = 7, .x = 35},
     // SHAPE_S 
     {.y = 6, .x = 40},
     // SHAPE_Q 
     {.y = 10, .x = 38},
     // SHAPE_H 
     {.y = 11, .x = 33},
     // SHAPE_F 
     {.y = 11, .x = 67},
     // SHAPE_I 
     {.y = 14, .x = 37},
     // SHAPE_L 
     {.y = 2, .x = 48},
     // SHAPE_N 
     {.y = 2, .x = 66},
     // SHAPE_P 
     {.y = 6, .x = 44},
     // SHAPE_T 
     {.y = 10, .x = 60},
     // SHAPE_U 
     {.y = 11, .x = 46},
     // SHAPE_V 
     {.y = 7, .x = 50},
     // SHAPE_W 
     {.y = 11, .x = 54},
     // SHAPE_X 
     {.y = 6, .x = 66},
     // SHAPE_Y 
     {.y = 2, .x = 56},
     // SHAPE_Z 
     {.y = 6, .x = 59},
};

const coord_t ALPHA_CURS[SHAPE_Z + 1] = {
    // SHAPE_O
    {.y = 14, .x = 68},
    // SHAPE_D
    {.y = 14, .x = 61},
    // SHAPE_M
    {.y = 14, .x = 53},
    // SHAPE_A
    {.y = 13, .x = 46},
    // SHAPE_E
    {.y = 0, .x = 36},
    // SHAPE_J
    {.y = 4, .x = 34},
    // SHAPE_S
    {.y = 4, .x = 39},
    // SHAPE_Q
    {.y = 9, .x = 39},
    // SHAPE_H
    {.y = 9, .x = 34},
    // SHAPE_F
    {.y = 9, .x = 67},
    // SHAPE_I
    {.y = 13, .x = 37},
    // SHAPE_L
    {.y = 0, .x = 45},
    // SHAPE_N
    {.y = 0, .x = 65},
    // SHAPE_P
    {.y = 4, .x = 45},
    // SHAPE_T
    {.y = 9, .x = 60},
    // SHAPE_U
    {.y = 9, .x = 46},
    // SHAPE_V
    {.y = 4, .x = 52},
    // SHAPE_W
    {.y = 9, .x = 54},
    // SHAPE_X
    {.y = 4, .x = 66},
    // SHAPE_Y
    {.y = 0, .x = 55},
    // SHAPE_Z
    {.y = 4, .x = 59},
};

int render_board(gcb_t *gcb)
{
    mvprintw(1, 0, "  0 1 2 3 4 5 6 7 8 9 a b c d  │\n");
    for (int y = 0; y < N_ROW; ++y) {
        printw("%x ", y);
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
    mvprintw(17, 0, "┌──────────────────┐");
    for (int i = 18; i <= 24; ++i) {
        mvprintw(i, 0, "│                  │");
    }
    mvprintw(25, 0, "└──────────────────┘");

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

int render_message_log(gcb_t *gcb);
