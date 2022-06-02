#ifndef _FRAME_H
#define _FRAME_H

/*
   |   0    |    1   | 2 - 17 |
   | opcode | status |  code  |
*/

enum OPCODE {
    REQ_PAIR,
    PAIRED,
    REQ_TURN,
    REQ_STATUS,
    RES,
    REQ_PLACE
};

#define OK 0
#define INV -1
#define BAD_REQ -2

#endif
