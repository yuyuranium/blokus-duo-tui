#ifndef _FRAME_H
#define _FRAME_H

/*
   |   0    |    1   | 2 - 17 |
   | opcode | status |  code  |
*/

#define FRAME_LEN 18
#define CODE_LEN 16

enum OPCODE {
    REQ_PAIR,
    PAIRED,
    REQ_TURN,
    TURN,
    REQ_STATUS,
    RES,
    REQ_PLACE,
    REQ_EOG,
    RES_EOG
};

#define RES_OK 0
#define RES_INV -1
#define RES_BAD_REQ -2

char *get_frame(int opcode, int status, char *code);
int parse_frame(char *frame, int *opcode, int *status, char *code);

#endif
