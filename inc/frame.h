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

#define RES_OK 0
#define RES_INV -1
#define RES_BAD_REQ -2

#endif
