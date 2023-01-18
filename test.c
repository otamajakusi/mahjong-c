#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "test.h"

bool test() {
    test_tile();
    test_meld();
    test_element();
    return true;
}

int main() {
    printf("mahjong\n");
    test();
    return 0;
}

