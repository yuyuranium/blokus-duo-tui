#include "frame.h"
#include <stdlib.h>
#include <string.h>

char *get_frame(int opcode, int status, char *code) 
{
    char *msg = malloc(FRAME_LEN);
    msg[0] = (char)opcode;
    msg[1] = (char)status;
    if (code) {
        strncpy(&msg[2], code, FRAME_LEN - 2);
    } else {
        memset(&msg[2], 0, FRAME_LEN - 2);
    }
    return msg;
}
