#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static int tile_cmp(const void *p1, const void *p2) {
    return *(const uint8_t*)p1 - *(const uint8_t*)p2;
}

int main() {
    uint8_t data[] = {4,5,7,1,2,8,9,3};
    qsort(data, sizeof(data), sizeof(data[0]), tile_cmp);
    for (int i = 0; i < sizeof(data); i ++) {
        printf("%d\n", data[i]);
    }
}

