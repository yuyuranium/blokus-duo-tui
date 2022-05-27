#include "inc/blokus.h"
#include <stdio.h>
#include <string.h>

void print_map(gcb_t *gcb)
{
    printf("  0 1 2 3 4 5 6 7 8 9 a b c d\n");
    for (int y = 0; y < N_ROW; ++y) {
        printf("%x ", y);
        for (int x = 0; x < N_COL; ++x) {
            char c;
            switch (gcb->map[y][x]) {
            case 0:
                c = 'o';
                break;
            case 1:
                c = 'x';
                break;
            case -1:
            default: 
                c = '.';
                break;
            }
            printf("%c ", c);
        }
        printf("\n");
    }
}

int main() 
{
    tile_t *tile;
    gcb_t *gcb = init_gcb(0);
    gcb_t *bbb = init_gcb(1);
    tile = sel_tile(gcb, SHAPE_W);
    tile->pos.y = 0x9; tile->pos.x = 0x4;
    printf("gcb\n");
    update(gcb, NULL);
    print_map(gcb);

    printf("bbb\n");
    update(bbb, gcb->code);
    print_map(bbb);

    printf("-=-");
    tile = sel_tile(bbb, SHAPE_Z);
    tile->pos.y = 0x9; tile->pos.x = 0x4;
    update(bbb, NULL);
    print_map(bbb);

    printf("gcb\n");
    update(gcb, bbb->code);
    print_map(gcb);



    return 0;
}
