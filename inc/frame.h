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

#define STATUS_INV -1
#define STATUS_OK 1
#define STATUS_FAILED 0

#endif
