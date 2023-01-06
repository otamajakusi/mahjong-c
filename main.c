#include "tile.h"
#include <assert.h>

static void test1() {
    Score score;
    Hands hands = {{1,2,3, 1,2,3, 4,5,6, 11,12,13, 41,41}, 14};
    Melds melds = {{{}},0};
    uint8_t win_tile = 41;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test2() {
    Score score;
    Hands hands = {{1,2,3, 1,2,3, 4,5,6, 11,12,13, 41,41}, 14};
    Melds melds = {
        {
            {{2,3,1,0}, 3, false},
        },
        1
    };
    uint8_t win_tile = 41;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test3() {
    Score score;
    Hands hands = {{1,2,3, 1,2,3, 4,5,6, 11,12,13, 41,41}, 14};
    Melds melds = {
        {
            {{ 2, 3, 1, 0}, 3, false},
            {{11,12,13, 0}, 3, false},
        },
        2
    };
    uint8_t win_tile = 41;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test4() {
    Score score;
    Hands hands = {{1,2,3, 7,7,7, 4,5,6, 11,12,13, 41,41}, 14};
    Melds melds = {
        {
            {{ 7, 7, 7, 0}, 3, false},
            {{11,12,13, 0}, 3, false},
        },
        2
    };
    uint8_t win_tile = 41;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}


static void test5() {
    Score score;
    Hands hands = {{1,2,3, 7,7,7, 4,5,6, 11,12,13, 41,41}, 14};
    Melds melds = {
        {
            {{ 7, 7, 7, 0}, 3, false},
            {{11,12,13, 0}, 3, false},
            {{41,41,41, 0}, 3, false}, // doesn't exist in hands
        },
        3
    };
    uint8_t win_tile = 41;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == ERR_ILLEGAL_PARAM);
}


static void test6() {
    Score score;
    Hands hands = {{1,2,3, 7,7,7,7, 4,5,6, 11,12,13, 41,41}, 14};
    Melds melds = {
        {
            {{ 7, 7, 7, 7}, 4, false},
            {{11,12,13, 0}, 3, false},
        },
        2
    };
    uint8_t win_tile = 41;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test7() {
    Score score;
    Hands hands = {{31,32,33, 7,7,7,7, 4,5,6, 11,12,13, 41,41}, 14};
    Melds melds = {
        {
            {{31,32,33, 0}, 3, false}, // winds tile can't be chi.
            {{11,12,13, 0}, 3, false},
        },
        2
    };
    uint8_t win_tile = 41;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == ERR_ILLEGAL_PARAM);
}


int main(int argc, char *argv[]) {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
}
