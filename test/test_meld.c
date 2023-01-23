#include <stdio.h>
#include <assert.h>

#include "test_meld.h"
#include "test_util.h"

void test_is_valid_melds() {
    MJMelds melds1 = {
        {
            {{m1,m2,m3}, 3, false, 0},
            {{m1,m2,m3}, 3, false, 0},
            {{m1,m2,m3}, 3, false, 0},
            {{m1,m2,m3}, 3, false, 0},
        },
        4
    };
    MJMelds melds2 = {
        {
            {{m1,m2,m3}, 3, false, 0},
            {{m1,m2,m3}, 3, false, 0},
        },
        2,
    };
    MJMelds melds3 = {
        {
            {{m1,m2,m3}, 3, false, 0},
            {{m1,m2,m3}, 3, false, 0},
            {{m1,m2,m3}, 3, false, 0},
            {{m1,m2,m3}, 3, false, 0},
        },
        5, /* INVALID */
    };
    MJMelds melds4 = {
        {
            {{m1,m2,m3}, 5, false, 0}, /* len is invalid */
        },
        1,
    };
    MJMelds melds5 = {
        {
            {{m1,m2,m3}, 2, false, 0}, /* len is invalid */
        },
        1,
    };
    MJMelds melds6 = {
        {
            {{m1,m2,dr+1}, 3, false, 0}, /* invalid tile_id */
        },
        1,
    };
    assert(is_valid_melds(&melds1));
    assert(is_valid_melds(&melds2));
    assert(!is_valid_melds(&melds3));
    assert(!is_valid_melds(&melds4));
    assert(!is_valid_melds(&melds5));
    assert(!is_valid_melds(&melds6));
}

bool test_meld() {
    test_is_valid_melds();
    return true;
}
