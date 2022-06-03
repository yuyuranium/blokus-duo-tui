#include "frame.h"
#include <stdlib.h>
#include <string.h>

char *get_frame(int opcode, int status, char *code) 
{
    char *frame = malloc(FRAME_LEN);
    frame[0] = (char)opcode;
    frame[1] = (char)status;
    if (code) {
        memcpy(&frame[2], code, CODE_LEN);
    } else {
        memset(&frame[2], 0, CODE_LEN);
    }
    return frame;
}

int parse_frame(char *frame, int *opcode, int *status, char *code)
{
    *opcode = (int)frame[0];
    *status = (int)frame[1];
    memcpy(code, &frame[2], CODE_LEN);
    return 0;
}
