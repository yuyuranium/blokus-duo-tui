#include "render.h"
#include <ncurses.h>

const coord_t TILE_CURS[SHAPE_Z + 1][5] = {
    {   // SHAPE_O 
        {.y = 15, .x = 68},
    },
    {   // SHAPE_R 
        {.y = 15, .x = 60},
        {.y = 15, .x = 62},
    },
    {   // SHAPE_M 
        {.y = 15, .x = 51},
        {.y = 15, .x = 53},
        {.y = 15, .x = 55},
    },
    {   // SHAPE_A 
        {.y = 14, .x = 45},
        {.y = 14, .x = 47},
        {.y = 15, .x = 47},
    },
    {   // SHAPE_E 
        {.y = 2, .x = 33},
        {.y = 2, .x = 35},
        {.y = 2, .x = 37},
        {.y = 2, .x = 39},
    },
    {   // SHAPE_J 
        {.y = 5, .x = 35},
        {.y = 6, .x = 35},
        {.y = 7, .x = 35},
        {.y = 7, .x = 33},
    },
    {   // SHAPE_S 
        {.y = 5, .x = 38},
        {.y = 6, .x = 38},
        {.y = 6, .x = 40},
        {.y = 7, .x = 40},
    },
    {   // SHAPE_Q 
        {.y = 10, .x = 38},
        {.y = 11, .x = 38},
        {.y = 10, .x = 40},
        {.y = 11, .x = 40},
    },
    {   // SHAPE_H 
        {.y = 10, .x = 33},
        {.y = 11, .x = 35},
        {.y = 11, .x = 33},
        {.y = 12, .x = 33},
    },
    {   // SHAPE_F 
        {.y = 10, .x = 65},
        {.y = 10, .x = 67},
        {.y = 11, .x = 69},
        {.y = 11, .x = 67},
        {.y = 12, .x = 67},
    },
    {   // SHAPE_I 
        {.y = 15, .x = 33},
        {.y = 15, .x = 35},
        {.y = 15, .x = 37},
        {.y = 15, .x = 39},
        {.y = 15, .x = 41},
    },
    {   // SHAPE_L 
        {.y = 1, .x = 48},
        {.y = 2, .x = 42},
        {.y = 2, .x = 44},
        {.y = 2, .x = 46},
        {.y = 2, .x = 48},
    },
    {   // SHAPE_N 
        {.y = 2, .x = 62},
        {.y = 2, .x = 64},
        {.y = 2, .x = 66},
        {.y = 1, .x = 66},
        {.y = 1, .x = 68},
    },
    {   // SHAPE_P 
        {.y = 5, .x = 44},
        {.y = 5, .x = 46},
        {.y = 6, .x = 44},
        {.y = 6, .x = 46},
        {.y = 7, .x = 44},
    },
    {   // SHAPE_T 
        {.y = 10, .x = 58},
        {.y = 10, .x = 60},
        {.y = 10, .x = 62},
        {.y = 11, .x = 60},
        {.y = 12, .x = 60},
    },
    {   // SHAPE_U 
        {.y = 10, .x = 44},
        {.y = 11, .x = 44},
        {.y = 11, .x = 46},
        {.y = 11, .x = 48},
        {.y = 10, .x = 48},
    },
    {   // SHAPE_V 
        {.y = 5, .x = 50},
        {.y = 6, .x = 50},
        {.y = 7, .x = 50},
        {.y = 7, .x = 52},
        {.y = 7, .x = 54},
    },
    {   // SHAPE_W 
        {.y = 10, .x = 52},
        {.y = 11, .x = 52},
        {.y = 11, .x = 54},
        {.y = 12, .x = 54},
        {.y = 12, .x = 56},
    },
    {   // SHAPE_X 
        {.y = 6, .x = 64},
        {.y = 6, .x = 66},
        {.y = 6, .x = 68},
        {.y = 5, .x = 66},
        {.y = 7, .x = 66},
    },
    {   // SHAPE_Y 
        {.y = 2, .x = 52},
        {.y = 2, .x = 54},
        {.y = 2, .x = 56},
        {.y = 2, .x = 58},
        {.y = 1, .x = 56},
    },
    {   // SHAPE_Z 
        {.y = 5, .x = 57},
        {.y = 5, .x = 59},
        {.y = 6, .x = 59},
        {.y = 7, .x = 59},
        {.y = 7, .x = 61},
    },
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
    {.y = 14, .x = 37},
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
const char ALPHAS[SHAPE_Z + 1] = {
    'M', 'D', 'R', 'A', 'E', 'J', 'S', 'Q', 'H', 'F', 'I', 'L', 'N', 'P', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' 
};
const char BLKCNT[SHAPE_Z + 1] = {
    1, 2, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};
int modify_board(gcb_t *gcb)
{
    mvprintw(1, 0, "  1 2 3 4 5 6 7 8 9 a b c d e  │\n");
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

int modify_tiles(gcb_t *gcb, int player)
{
    for (int i = 0; i < SHAPE_Z + 1; ++i) {
        if (gcb->hand[player][i]) {
            mvprintw(ALPHA_CURS[i].y, ALPHA_CURS[i].x, "%c", ALPHAS[i]);
            for (int j = 0; j < BLKCNT[i]; ++j) {
                mvprintw(TILE_CURS[i][j].y, TILE_CURS[i][j].x, "%s",
                        (player)? "□" : "■");
            }
        } else {
            mvprintw(ALPHA_CURS[i].y, ALPHA_CURS[i].x, " ");
            for (int j = 0; j < BLKCNT[i]; ++j) {
                mvprintw(TILE_CURS[i][j].y, TILE_CURS[i][j].x, " ");
            }
        }
    }
    return 0; 
}
int modify_chosen_tile(gcb_t *gcb);
int modify_message_log(gcb_t *gcb);
