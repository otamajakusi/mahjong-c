#include "tile.h"
#include <assert.h>
#include <stdio.h>

// alias
enum {
    m1 = MAN1,
    m2 = MAN2,
    m3 = MAN3,
    m4 = MAN4,
    m5 = MAN5,
    m6 = MAN6,
    m7 = MAN7,
    m8 = MAN8,
    m9 = MAN9,
    p1 = PIN1,
    p2 = PIN2,
    p3 = PIN3,
    p4 = PIN4,
    p5 = PIN5,
    p6 = PIN6,
    p7 = PIN7,
    p8 = PIN8,
    p9 = PIN9,
    s1 = SOU1,
    s2 = SOU2,
    s3 = SOU3,
    s4 = SOU4,
    s5 = SOU5,
    s6 = SOU6,
    s7 = SOU7,
    s8 = SOU8,
    s9 = SOU9,
    wt = TON,
    wn = NAN,
    ws = SHA,
    wp = PEI,
    d1 = HAKU,
    d2 = HATSU,
    d3 = CHUN,
};

static void test1() {
    Score score;
    Hands hands = {{m1,m2,m3, m1,m2,m3, m4,m5,m6, p1,p2,p3, wt,wt}, 14};
    Melds melds = {{{}},0};
    uint8_t win_tile = wt;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test2() {
    Score score;
    Hands hands = {{m1,m2,m3, m1,m2,m3, m4,m5,m6, p1,p2,p3, wt,wt}, 14};
    Melds melds = {
        {
            {{m2,m3,m1,INV}, 3, false},
        },
        1
    };
    uint8_t win_tile = wt;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test3() {
    Score score;
    Hands hands = {{m1,m2,m3, m1,m2,m3, m4,m5,m6, p1,p2,p3, wt,wt}, 14};
    Melds melds = {
        {
            {{m2,m3,m1, INV}, 3, false},
            {{p1,p2,p3, INV}, 3, false},
        },
        2
    };
    uint8_t win_tile = wt;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test4() {
    Score score;
    Hands hands = {{m1,m2,m3, m7,m7,m7, m4,m5,m6, p1,p2,p3, wt,wt}, 14};
    Melds melds = {
        {
            {{m7,m7,m7, INV}, 3, false},
            {{p1,p2,p3, INV}, 3, false},
        },
        2
    };
    uint8_t win_tile = wt;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}


static void test5() {
    Score score;
    Hands hands = {{m1,m2,m3, m7,m7,m7, m4,m5,m6, p1,p2,p3, wt,wt}, 14};
    Melds melds = {
        {
            {{m7,m7,m7, INV}, 3, false},
            {{p1,p2,p3, INV}, 3, false},
            {{wt,wt,wt, INV}, 3, false}, // doesn't exist in hands
        },
        3
    };
    uint8_t win_tile = wt;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == ERR_ILLEGAL_PARAM);
}


static void test6() {
    Score score;
    Hands hands = {{m1,m2,m3, m7,m7,m7,m7, m4,m5,m6, p1,p2,p3, wt,wt}, 15};
    Melds melds = {
        {
            {{m7,m7,m7,m7}, 4, false},
            {{p1,p2,p3, INV}, 3, false},
        },
        2
    };
    uint8_t win_tile = wt;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test7() {
    Score score;
    Hands hands = {{wt,wn,ws, m7,m7,m7, m4,m5,m6, p1,p2,p3, wt,wt}, 14};
    Melds melds = {
        {
            {{wt,wn,ws, INV}, 3, false}, // winds tile can't be chi.
            {{p1,p2,p3, INV}, 3, false},
        },
        2
    };
    uint8_t win_tile = wt;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == ERR_ILLEGAL_PARAM);
}

static void test8() {
    Score score;
    Hands hands = {{m8,m9,m7, m7,m7,m7, m4,m5,m6, p1,p2,p3, wt,wt}, 14};
    Melds melds = {
        {
            {{m8,m9,p1, INV}, 3, false}, // m8-m9-p1 can't be chi
            {{p1,p2,p3, INV}, 3, false},
        },
        2
    };
    uint8_t win_tile = wt;

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
    test8();
}
