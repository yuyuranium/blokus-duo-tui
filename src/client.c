#include "client.h"
#include "blokus.h"
#include "sock.h"
#include "frame.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <sys/socket.h>
#include <time.h>

int choose_tile_handler(int c, rcb_t *rcb, char *msg[7], int *color[7])
{
    if (rcb->render_player == 0) {
        recover_tile_preview(rcb->gcb,
                             tile_relation[rcb->coord.y][rcb->coord.x]);
    }
    gcb_t* gcb = rcb->gcb;
    switch (c) {
    case 'h':
    case KEY_LEFT:
        if (rcb->coord.x > 0) {
            int x = rcb->coord.x - 1;
            if (tile_relation[rcb->coord.y][rcb->coord.x] == 
                tile_relation[rcb->coord.y][x]) x--;
            while (x >= 0 && !gcb->hand[0][tile_relation[rcb->coord.y][x]]) x--;
            rcb->coord.x = (x >= 0)? x : rcb->coord.x;
        }
        break;
    case 'j':
    case KEY_DOWN:
        if (rcb->coord.y < 3) {
            int y = rcb->coord.y + 1;
            while (y <= 3 && !gcb->hand[0][tile_relation[y][rcb->coord.x]]) y++;
            rcb->coord.y = (y <= 3)? y : rcb->coord.y;
        }
        break;
    case 'k':
    case KEY_UP:
        if (rcb->coord.y > 0) {
            int y = rcb->coord.y - 1;
            while (y >= 0 && !gcb->hand[0][tile_relation[y][rcb->coord.x]]) y--;
            rcb->coord.y = (y >= 0)? y : rcb->coord.y;
        }
        break;
    case 'l':
    case KEY_RIGHT:
        if (rcb->coord.x < 5) {
            int x = rcb->coord.x + 1;
            if (tile_relation[rcb->coord.y][rcb->coord.x] == 
                tile_relation[rcb->coord.y][x]) x++;
            while (x <= 5 && !gcb->hand[0][tile_relation[rcb->coord.y][x]]) x++;
            rcb->coord.x = (x <= 5)? x : rcb->coord.x;
        }
        break;
    case 'c':
    case 'C':
        rcb->render_player = !rcb->render_player;
        render_tiles(rcb->gcb, rcb->render_player);
        break;
    case ' ': {
        tile_t *chosen = sel_tile(rcb->gcb,
                                  tile_relation[rcb->coord.y][rcb->coord.x]);
        chosen->pos.x = 7;
        chosen->pos.y = 7;
        rcb->chosen = chosen;
        rcb->state++;
        render_board_preview(rcb);
        break;
    }
    case 'H':
        shift_msg(msg, color);
        snprintf(msg[6], MAX_LOG_LEN,
                 "[Hint] You could place tile \"%c\" at (%x, %x)",
                 TILE[gcb->hint.shape].alpha, gcb->hint.pos.y, gcb->hint.pos.x);
        *color[6] = BLUE_PAIR;
        render_message_log(msg, color);
        break;
    default:
        if (isprint(c)) {
            shift_msg(msg, color);
            snprintf(msg[6], MAX_LOG_LEN, "[Warning] Invalid operation \"%c\"", c);
            *color[6] = YELLOW_PAIR;
            render_message_log(msg, color);
        }
        break;
    }
    if (gcb->turn == 0) {
        render_tile_preview(rcb->gcb,
                            tile_relation[rcb->coord.y][rcb->coord.x]);
    }
    return 0;
}

int positioning_handler(int c, rcb_t *rcb, char *msg[7], int *color[7])
{
    recover_board_preview(rcb);
    tile_t *chosen = rcb->chosen;
    gcb_t *gcb = rcb->gcb;
    coord_t tmp_pos = chosen->pos;
    int operation = 0;
    switch (c) {
    case 'h':
    case KEY_LEFT:
        chosen->pos.x--;
        break;
    case 'j':
    case KEY_DOWN:
        chosen->pos.y++;
        break;
    case 'k':
    case KEY_UP:
        chosen->pos.y--;
        break;
    case 'l':
    case KEY_RIGHT:
        chosen->pos.x++;
        break;
    case 'c':
    case 'C':
        rcb->render_player = !rcb->render_player;
        render_tiles(gcb, rcb->render_player);
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
    case ' ':
        shift_msg(msg, color);
        if (!is_valid(gcb)) {
            snprintf(msg[6], MAX_LOG_LEN,
                     "[Warning] You cannot place the tile here"); 
            *color[6] = YELLOW_PAIR;
            render_message_log(msg, color);
            break;
        }
        rcb->state++;
        snprintf(msg[6], MAX_LOG_LEN,
                 "Do you want to place at (%x, %x)? (Y/n)",
                 chosen->pos.y, chosen->pos.x);
        render_message_log(msg, color);
        break;
    case 'H':
        shift_msg(msg, color);
        snprintf(msg[6], MAX_LOG_LEN,
                 "[Hint] You could place tile \"%c\" at (%x, %x)",
                 TILE[gcb->hint.shape].alpha, gcb->hint.pos.y, gcb->hint.pos.x);
        *color[6] = BLUE_PAIR;
        render_message_log(msg, color);
        break;
    default:
        if (isprint(c)) {
            shift_msg(msg, color);
            snprintf(msg[6], MAX_LOG_LEN, "[Warning] Invalid operation \"%c\"", c);
            *color[6] = YELLOW_PAIR;
            render_message_log(msg, color);
        }
        break;
    }
    for (int i = 0; i < TILE[chosen->shape].blk_cnt; ++i) {
        if (chosen->pos.x + chosen->blks[i].x < N_COL &&
            chosen->pos.x + chosen->blks[i].x >= 0 &&
            chosen->pos.y + chosen->blks[i].y < N_ROW &&
            chosen->pos.y + chosen->blks[i].y >= 0)
            continue;
            
        chosen->pos = tmp_pos;
        if (operation) {
            shift_msg(msg, color);
            switch (operation) {
            case 1:
                snprintf(msg[6], MAX_LOG_LEN,
                         "[Warning] You cannot do rotation in this position");
                rot_tile(chosen, 270);
                break;
            case 2:
                snprintf(msg[6], MAX_LOG_LEN,
                         "[Warning] You cannot do rotation in this position");
                rot_tile(chosen, 90);
                break;
            case 3:
                snprintf(msg[6], MAX_LOG_LEN,
                         "[Warning] You cannot mirror the tile here");
                mir_tile(chosen);
                break;
            }
            *color[6] = YELLOW_PAIR;
            render_message_log(msg, color);
        }
        break;
    }
    render_board_preview(rcb);
    return 0;
}

int placing_handler(int c, rcb_t *rcb, char *msg[7], int *color[7])
{
    gcb_t *gcb = rcb->gcb;
    switch (c) {
    case 'y':
    case 'Y':
    case 10:
        shift_msg(msg, color);
        if (update(gcb, 0) < 0) {
            snprintf(msg[6], MAX_LOG_LEN, "[Error] Board update failed"); 
            *color[6] = RED_PAIR;
            rcb->state = S_POSITIONING;
            render_message_log(msg, color);
            return -1;
        } else {
            snprintf(msg[6], MAX_LOG_LEN, "You have put the tile successfully"); 
            *color[6] = GREEN_PAIR;
            render_board(gcb);
            render_tiles(gcb, gcb->turn);
            render_score(rcb);
            rcb->state = S_CHOOSE_TILE;

            render_message_log(msg, color);
            return 0;
        }
        break;
    case 'q':
    case 'n':
    case 'N':
        shift_msg(msg, color);
        snprintf(msg[6], MAX_LOG_LEN, "Tile placement canceled"); 
        *color[6] = YELLOW_PAIR;
        render_message_log(msg, color);
        rcb->state = S_POSITIONING;
        return -1;
        break;
    } 
    return 0;
}

int game_over_handler(rcb_t *rcb, char *msgs[7], int *color[7])
{
    gcb_t *gcb = rcb->gcb;
    shift_msg(msgs, color);
    switch (gcb->status) {
    case EOG_P:
        snprintf(msgs[6], MAX_LOG_LEN, ":: GAME OVER! You have won the game! ::"); 
        *color[6] = GREEN_PAIR;
    case EOG_Q:
        snprintf(msgs[6], MAX_LOG_LEN, ":: GAME OVER! You have lose the game! ::"); 
        *color[6] = RED_PAIR;
    case EOG_T:
        snprintf(msgs[6], MAX_LOG_LEN, ":: GAME OVER! Tie game! ::"); 
        *color[6] = YELLOW_PAIR;
    default:
        return -1;
    }
    do {
        shift_msg(msgs, color);
        snprintf(msgs[6], MAX_LOG_LEN, "Press 'n' to start new game."); 
        shift_msg(msgs, color);
        snprintf(msgs[6], MAX_LOG_LEN, "Press 'q' to quit the game."); 
        render_message_log(msgs, color);

        int c = getch();
        if (c == ERR) continue;
        switch (c) {
        case 'c':
            return 0;
        case 'q':
            exit(0);
        }
    } while (1);
    
    return 0;
}

int main(int argc, char *argv[])
{
    char host[30] = {0};
    char port[5] = {0};
    
    --argc; ++argv;
    if (argc > 0 && **argv == '-' && (*argv)[1] == 'h') {
        --argc; ++argv;
        if (argc < 1)
            return -1;
        strncpy(host, *argv, strlen(*argv));
        printf("%s\n", host);
    }
    
    --argc; ++argv;
    if (argc > 0 && **argv == '-' && (*argv)[1] == 'p') {
        --argc; ++argv;
        if (argc < 1)
            return -1;
        strncpy(port, *argv, strlen(*argv));
        printf("%s\n", port);
    }

    if (host[0] == 0 || port[0] == 0) {
        printf("[Error] Server host or port not specified. Exit game.\n");
        exit(-1);
    }

    int client_fd __attribute__((unused)) = open_clientfd(host, port);
    if (client_fd == -1) {
        printf("[Error] Client file descriptor open failed.\n");
        printf("[Error] Please check host and port again.\n");
        exit(-1);
    }

NEW_GAME:
    setlocale(LC_ALL, "");
    initscr();
    init_all_colors();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    clear();

    // Pairing
    int win_row, win_col;
    getmaxyx(stdscr, win_row, win_col);
    attron(A_BLINK);
    mvprintw(win_row / 2, (win_col - 21) / 2, "Pairing Opponents ...");
    attroff(A_BLINK);
    mvprintw(win_row / 2 + 3, 
             (win_col - strlen(host) - strlen(port) - 9) / 2, 
             "server: %s:%s", host, port);
    refresh();
    // set request reqest pair signal
    char *frame = get_frame(REQ_PAIR, 0, NULL);
    char *recv_frame = malloc(FRAME_LEN);
    int opcode;
    int status;
    char* code = malloc(CODE_LEN);
    while (1) {
        send(client_fd, frame, FRAME_LEN, 0);
        if (recv(client_fd, recv_frame, FRAME_LEN, 0) > 0 &&
            !parse_frame(recv_frame, &opcode, &status, code) &&
            opcode == PAIRED && status == RES_OK) {
            if (code[0] == 1) {  // pair success
                break;
            }
        }
        clock_t begin = clock();
        while (clock() - begin < TIMEOUT);
    }
    clear();

    gcb_t* gcb;
    // Request turn
    frame = get_frame(REQ_TURN, 0, NULL);
    while (1) {
        send(client_fd, frame, FRAME_LEN, 0);
        if (recv(client_fd, recv_frame, FRAME_LEN, 0) > 0 &&
            !parse_frame(recv_frame, &opcode, &status, code) &&
            opcode == TURN && status == RES_OK) {
            if (code[0] == 0) {
                gcb = init_gcb(0);
                break;
            } else if (code[0] == 1) {
                gcb = init_gcb(1);
                break;
            }
        }
        clock_t begin = clock();
        while (clock() - begin < TIMEOUT);
    }
    
    char *strs[7];
    int *colors[7];
    for (int i = 0; i < 7; ++i) {
        strs[i] = malloc(sizeof(char) * 70);
        colors[i] = malloc(sizeof(int));
        *colors[i] = 0; 
    }
    
    rcb_t *rcb = malloc(sizeof(rcb_t));
    rcb->gcb = gcb;
    rcb->render_player = 0;
    rcb->state = 0;
    rcb->coord.x = 0;
    rcb->coord.y = 0;
    
    render_board(gcb);
    render_tiles(gcb, rcb->render_player);
    render_message_log(strs, colors);
    render_tile_preview(gcb, SHAPE_E);
    render_score_board();
    render_score(rcb);
    
    do {
        refresh();

        if (gcb->turn == 0) {
            int c = getch();
            if (c == ERR) continue;
            switch (rcb->state) {
                case S_CHOOSE_TILE:
                    choose_tile_handler(c, rcb, strs, colors);
                    break;
                case S_POSITIONING:
                    positioning_handler(c, rcb, strs, colors);
                    break;
                case S_PLACING:
                    if (placing_handler(c, rcb, strs, colors) == 0) {
                        frame = get_frame(REQ_PLACE, 0, gcb->code);
                        while (1) {
                            send(client_fd, frame, FRAME_LEN, 0);
                            if (recv(client_fd, recv_frame, FRAME_LEN, 0) > 0) {
                                parse_frame(recv_frame, &opcode, &status, code);
                                if (opcode == RES && status == RES_OK) {
                                    break;
                                }
                            }
                            clock_t begin = clock();
                            while (clock() - begin < TIMEOUT);
                        }
                    }
                    break;
            }
        } else {  // wait for other player
WAIT: 
            frame = get_frame(REQ_STATUS, 0, NULL);
            while (1) {
                send(client_fd, frame, FRAME_LEN, 0);
                if (recv(client_fd, recv_frame, FRAME_LEN, 0) > 0) {
                    parse_frame(recv_frame, &opcode, &status, code);
                    if (opcode == RES && status == RES_OK &&
                        !update(gcb, code)) {
                        break;
                    }
                }
                clock_t begin = clock();
                while (clock() - begin < TIMEOUT);
            }
            
            render_board(gcb);
            render_tiles(gcb, gcb->turn);
            render_score_board();
            render_score(rcb);
            
            // find next start candidate tile
            int tile_found = 0;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 6; ++j) {
                    if (gcb->hand[0][tile_relation[i][j]]) {
                        rcb->coord.x = j;
                        rcb->coord.y = i;
                        tile_found = 1;
                        break;
                    }
                }
                if (tile_found) break;
            }
            if (gcb->status == EOG_P ||
                gcb->status == EOG_Q ||
                gcb->status == EOG_T) {
                memset(code, 0, FRAME_LEN);
                code[0] = gcb->status;
                frame = get_frame(REQ_EOG, 0, code);
                while (1) {
                    send(client_fd, frame, FRAME_LEN, 0);
                    if (recv(client_fd, recv_frame, FRAME_LEN, 0) > 0) {
                        parse_frame(recv_frame, &opcode, &status, code);
                        if (opcode == RES_EOG && status == RES_OK) {
                            break;
                        }
                    }
                    goto WAIT;
                }
                int game_result = game_over_handler(rcb, strs, colors);
                free(frame);
                free(recv_frame);
                free(rcb->gcb);
                free(rcb);
                free(code);
                for (int i = 0; i < 7; ++i) {
                    free(strs[i]);
                    free(colors[i]);
                }
                if (!game_result) {
                    goto NEW_GAME;
                } else {
                    exit(0);
                }
            }
            render_tile_preview(gcb, tile_relation[rcb->coord.y][rcb->coord.x]);
        }
    } while (1);
    
    endwin();
    return 0;
}
