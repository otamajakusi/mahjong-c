#include <stdio.h>
#include <assert.h>

#include "test_hand.h"
#include "test_util.h"

void test_is_valid_hands() {
    MJHands hands1 = {
        {m1,m2,m3,m4,m5,m6,m7,m8,m9,m7,m8,m9,dw,dw},
        3 * 4 + 2,
    };
    MJHands hands2 = {
        {m1,m2,m3,m4,m5,m6,m7,m8,m9,m7,m8,m9,m1,m2,m3,m4,dw,dw},
        4 * 4 + 2,
    };
    MJHands hands3 = {
        {m1,m2,m3,m4,m5,m6,m7,m8,m9,m7,m8,m9,dw,dw},
        3 * 4 + 2 - 1,
    };
    MJHands hands4 = {
        {m1,m2,m3,m4,m5,m6,m7,m8,m9,m7,m8,m9,m1,m2,m3,m4,dw,dw},
        4 * 4 + 2 + 1, // invalid len
    };
    MJHands hands5 = {
        {m1,m2,m3,m4,m5,m6,m7,m8,m9,m7,m8,m9,m1,m2,m3,m4,dw,dr+1}, // illegal tile id
        4 * 4 + 2,
    };
    assert(is_valid_hands(&hands1));
    assert(is_valid_hands(&hands2));
    assert(!is_valid_hands(&hands3));
    assert(!is_valid_hands(&hands4));
    assert(!is_valid_hands(&hands5));
}

bool test_hand() {
    test_is_valid_hands();
    return true;
}
