#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "test.h"

bool test() {
    test_tile();
    return true;
}

int main() {
    printf("mahjong\n");
    test();
    return 0;
}

