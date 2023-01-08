/*
 *  MIT License
 *  
 *  Copyright (c) 2023 otamajakusi
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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
    dw = HAKU,
    dg = HATSU,
    dr = CHUN,
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
            {{wt,wn,ws, INV}, 3, false}, // winds tile can't be meld.
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
            {{m8,m9,p1, INV}, 3, false}, // m8-m9-p1 can't be meld
            {{p1,p2,p3, INV}, 3, false},
        },
        2
    };
    uint8_t win_tile = wt;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == ERR_ILLEGAL_PARAM);
}

static void test9() {
    Score score;
    Hands hands = {{m1,m1,m2,m2,m3,m3,m4,m4, p1,p2,p3, s1,s2,s3}, 14}; // both m1 and m4 can be head
    Melds melds = {
        {
        },
        0
    };
    uint8_t win_tile = s3;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test10() {
    Score score;
    Hands hands = {{m1,m1,m2,m2,m3,m3,m4,m4, p1,p1,p1, s1,s2,s3}, 14}; // both m1 and m4 can be head
    Melds melds = {
        {
        },
        0
    };
    uint8_t win_tile = s3;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test11() {
    Score score;
    Hands hands = {{m2,m3,m3,m3,m3,m4,m4,m4,m5,m5,m6,m6,m8,m8}, 14}; // m3 and m4 can be kotsu candidate
    Melds melds = {
        {
        },
        0
    };
    uint8_t win_tile = m8;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test12() {
    Score score;
    Hands hands = {{m5,m5,m5,m6,m6,m6,m6,m7,m7,m7,m8,m8,m9,m9}, 14};
    Melds melds = {
        {
        },
        0
    };
    uint8_t win_tile = m8;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

static void test13() {
    Score score;
    Hands hands = {{m6,m6,m7,m7,m8,m8,m9,m9,p7,p8,p9,s7,s8,s9}, 14}; // multiple agari type
    Melds melds = {
        {
        },
        0
    };
    uint8_t win_tile = m8;

    assert(tile_get_score(&score, &hands, &melds, win_tile, true) == OK);
}

int main(int argc, char *argv[]) {
    printf("---test1\n"); test1();
    printf("---test2\n"); test2();
    printf("---test3\n"); test3();
    printf("---test4\n"); test4();
    printf("---test5\n"); test5();
    printf("---test6\n"); test6();
    printf("---test7\n"); test7();
    printf("---test8\n"); test8();
    printf("---test9\n"); test9();
    printf("---test10\n"); test10();
    printf("---test11\n"); test11();
    printf("---test12\n"); test12();
    printf("---test13\n"); test13();
}
