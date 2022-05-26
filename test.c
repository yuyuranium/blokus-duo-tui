#include "inc/blokus.h"
#include "inc/render.h"
#include "inc/client.h"
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>

int main() {
    
    setlocale(LC_ALL, "");
    initscr();
    init_all_colors();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    
    gcb_t *game = malloc(sizeof(gcb_t));
    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 14; ++x) {
            game->map[y][x] = -1;
        }
    }
    for (int i = 0; i < 21; ++i) {
        game->hand[0][i] = 1;
        game->hand[1][i] = 1;
    }
    char *strs[6];
    int str_len[6] = {0};
    for (int i = 0; i < 6; ++i) {
        strs[i] = malloc(sizeof(char) * 54);
    }
    
    rcb_t *rcb = malloc(sizeof(rcb_t));
    tile_t *tile = make_tile(SHAPE_W);
    rcb->gcb = game;
    rcb->chosen = tile;
    rcb->render_player = 0;
    rcb->state = 0;
    
    render_board(game);
    render_tiles(game, rcb->render_player);
    render_chosen_tile(tile);
    render_message_log(strs, str_len);
    render_tile_preview(game, SHAPE_E);
    
    coord_t *coord = malloc(sizeof(coord_t));
    coord->x = 0;
    coord->y = 0;
    do {
        refresh();
        int c = getch();
        if (c == ERR) continue;
        switch (rcb->state) {
            case S_CHOOSE_TILE:
                choose_tile_handler(c, rcb, coord);
                break;
            case S_POSITIONING:
                positioning_handler(c, rcb);
                break;
        }
    } while (1);
    

    
    // getstr(strs[5]);
    // str_len[5] = strlen(strs[5]);
    // for (int x = 0; x < 14; ++x) {
    //     game->map[6][x] = 0;
    // }
    // rot_tile(tile, 90);
    // render_board(game);
    // render_tiles(game, 0);
    // render_chosen_tile(tile);
    // render_message_log(strs, str_len);
    // refresh();
    // shift_msg(strs, str_len);


    // 
    // getstr(strs[5]);
    // str_len[5] = strlen(strs[5]);
    // for (int y = 0; y < 14; ++y) {
    //     game->map[y][6] = 1;
    // }
    // render_board(game);
    // render_tiles(game, 1);
    // render_message_log(strs, str_len);
    // refresh();
    // shift_msg(strs, str_len);

    // 
    // for (int i = 0; i < 21; ++i) {
    //     getstr(strs[5]);
    //     str_len[5] = strlen(strs[5]);
    //     tile = make_tile(i);
    //     game->hand[1][i] = 0;
    //     render_tiles(game, 1);
    //     render_chosen_tile(tile);
    //     render_message_log(strs, str_len);
    //     refresh();
    //     shift_msg(strs, str_len);
    // }

    
    endwin();

    return 0;
}
