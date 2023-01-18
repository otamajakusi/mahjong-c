#include <stdio.h>
#include <assert.h>

#include "test_tile.h"
#include "test_util.h"

static void test_is_tile_id_valid() {
    assert(is_tile_id_valid(m1));
    assert(is_tile_id_valid(dr));
    assert(!is_tile_id_valid(-1));
    assert(!is_tile_id_valid(dr+1));
}

static void test_is_tile_id_honors() {
    assert(is_tile_id_honors(wt));
    assert(is_tile_id_honors(wn));
    assert(is_tile_id_honors(ws));
    assert(is_tile_id_honors(wp));
    assert(is_tile_id_honors(dw));
    assert(is_tile_id_honors(dg));
    assert(is_tile_id_honors(dr));
    assert(!is_tile_id_honors(m1));
    assert(!is_tile_id_honors(s9));
}

static void test_is_tile_id_wind() {
    assert(is_tile_id_wind(wt));
    assert(is_tile_id_wind(wn));
    assert(is_tile_id_wind(ws));
    assert(is_tile_id_wind(wp));
    assert(!is_tile_id_wind(m1));
    assert(!is_tile_id_wind(s1));
    assert(!is_tile_id_wind(s9));
    assert(!is_tile_id_wind(dr));
}

static void test_is_tile_id_dragon() {
    assert(is_tile_id_dragon(dw));
    assert(is_tile_id_dragon(dg));
    assert(is_tile_id_dragon(dr));
    assert(!is_tile_id_dragon(dr+1));
    assert(!is_tile_id_dragon(wp));
    assert(!is_tile_id_dragon(m1));
    assert(!is_tile_id_dragon(s9));
}

static void test_is_tile_id_man() {
    assert(is_tile_id_man(m1));
    assert(is_tile_id_man(m2));
    assert(is_tile_id_man(m3));
    assert(is_tile_id_man(m4));
    assert(is_tile_id_man(m5));
    assert(is_tile_id_man(m6));
    assert(is_tile_id_man(m7));
    assert(is_tile_id_man(m8));
    assert(is_tile_id_man(m9));
    assert(!is_tile_id_man(m1-1));
    assert(!is_tile_id_man(p1));
    assert(!is_tile_id_man(wt));
    assert(!is_tile_id_man(dw));
    assert(!is_tile_id_man(dr));
}

static void test_is_tile_id_pin() {
    assert(is_tile_id_pin(p1));
    assert(is_tile_id_pin(p2));
    assert(is_tile_id_pin(p3));
    assert(is_tile_id_pin(p4));
    assert(is_tile_id_pin(p5));
    assert(is_tile_id_pin(p6));
    assert(is_tile_id_pin(p7));
    assert(is_tile_id_pin(p8));
    assert(is_tile_id_pin(p9));
    assert(!is_tile_id_pin(p1-1));
    assert(!is_tile_id_pin(m1));
    assert(!is_tile_id_pin(s1));
    assert(!is_tile_id_pin(wt));
    assert(!is_tile_id_pin(dw));
    assert(!is_tile_id_pin(dr));
}

static void test_is_tile_id_sou() {
    assert(is_tile_id_sou(s1));
    assert(is_tile_id_sou(s2));
    assert(is_tile_id_sou(s3));
    assert(is_tile_id_sou(s4));
    assert(is_tile_id_sou(s5));
    assert(is_tile_id_sou(s6));
    assert(is_tile_id_sou(s7));
    assert(is_tile_id_sou(s8));
    assert(is_tile_id_sou(s9));
    assert(!is_tile_id_sou(s1-1));
    assert(!is_tile_id_sou(m1));
    assert(!is_tile_id_sou(p1));
    assert(!is_tile_id_sou(wt));
    assert(!is_tile_id_sou(dw));
    assert(!is_tile_id_sou(dr));
}

/* 1,9字牌 */
static void test_is_tile_id_yaochu() {
    assert(is_tile_id_yaochu(m1));
    assert(is_tile_id_yaochu(m9));
    assert(is_tile_id_yaochu(p1));
    assert(is_tile_id_yaochu(p9));
    assert(is_tile_id_yaochu(s1));
    assert(is_tile_id_yaochu(s9));
    assert(is_tile_id_yaochu(wt));
    assert(is_tile_id_yaochu(wp));
    assert(is_tile_id_yaochu(dw));
    assert(is_tile_id_yaochu(dr));
    assert(!is_tile_id_yaochu(m2));
    assert(!is_tile_id_yaochu(m8));
    assert(!is_tile_id_yaochu(p2));
    assert(!is_tile_id_yaochu(p8));
    assert(!is_tile_id_yaochu(s2));
    assert(!is_tile_id_yaochu(s8));
}

/* 1,9牌 */
static void test_is_tile_id_routou() {
    assert(is_tile_id_routou(m1));
    assert(is_tile_id_routou(m9));
    assert(is_tile_id_routou(p1));
    assert(is_tile_id_routou(p9));
    assert(is_tile_id_routou(s1));
    assert(is_tile_id_routou(s9));
    assert(!is_tile_id_routou(wt));
    assert(!is_tile_id_routou(wp));
    assert(!is_tile_id_routou(dw));
    assert(!is_tile_id_routou(dr));
    assert(!is_tile_id_routou(m2));
    assert(!is_tile_id_routou(m8));
    assert(!is_tile_id_routou(p2));
    assert(!is_tile_id_routou(p8));
    assert(!is_tile_id_routou(s2));
    assert(!is_tile_id_routou(s8));
}


static void test_get_tile_type() {
    assert(get_tile_type(m1) == TILE_TYPE_MAN);
    assert(get_tile_type(m9) == TILE_TYPE_MAN);
    assert(get_tile_type(p1) == TILE_TYPE_PIN);
    assert(get_tile_type(p9) == TILE_TYPE_PIN);
    assert(get_tile_type(s1) == TILE_TYPE_SOU);
    assert(get_tile_type(s9) == TILE_TYPE_SOU);
    assert(get_tile_type(wt) == TILE_TYPE_WIND);
    assert(get_tile_type(wn) == TILE_TYPE_WIND);
    assert(get_tile_type(ws) == TILE_TYPE_WIND);
    assert(get_tile_type(wp) == TILE_TYPE_WIND);
    assert(get_tile_type(dw) == TILE_TYPE_DRAGON);
    assert(get_tile_type(dg) == TILE_TYPE_DRAGON);
    assert(get_tile_type(dr) == TILE_TYPE_DRAGON);
    assert(get_tile_type(-1) == TILE_TYPE_INVALID);
}

static void test_get_tile_number() {
    assert(get_tile_number(m1) == TILE_NUM_1);
    assert(get_tile_number(m9) == TILE_NUM_9);
    assert(get_tile_number(p2) == TILE_NUM_2);
    assert(get_tile_number(p8) == TILE_NUM_8);
    assert(get_tile_number(s3) == TILE_NUM_3);
    assert(get_tile_number(s7) == TILE_NUM_7);
    assert(get_tile_number(wt) == TILE_NUM_INVALID);
}

__attribute__((unused)) static void dump(const uint8_t *p, size_t size) {
    for (size_t i = 0; i < size; i ++) {
        fprintf(stderr, "%02x,", p[i]);
        if ((i + 1) % 16 == 0) {
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n");
}

static void test_gen_tiles_from_hands() {
    MJHands hands = {{m1,m2,m3, wt,wt,wt, p7,p8,p9, s4,s5,s6, dw,dw}, 14};
    Tiles exp = {{
        1,1,1,0,0,0,0,0,0,
        0,0,0,0,0,0,1,1,1,
        0,0,0,1,1,1,0,0,0,
        3,0,0,0,
        2,0,0
    }};
    Tiles act;
    memset(&act, -1, sizeof(Tiles));
    gen_tiles_from_hands(&act, &hands);
    assert(memcmp(&act, &exp, sizeof(Tiles)) == 0);
}

static void test_tile_id_str() {
    assert(strcmp(tile_id_str(m1), "m1") == 0);
    assert(strcmp(tile_id_str(m9), "m9") == 0);
    assert(strcmp(tile_id_str(p1), "p1") == 0);
    assert(strcmp(tile_id_str(p9), "p9") == 0);
    assert(strcmp(tile_id_str(s1), "s1") == 0);
    assert(strcmp(tile_id_str(s9), "s9") == 0);
    assert(strcmp(tile_id_str(wt), "wt") == 0);
    assert(strcmp(tile_id_str(wp), "wp") == 0);
    assert(strcmp(tile_id_str(dw), "dw") == 0);
    assert(strcmp(tile_id_str(dr), "dr") == 0);
}

bool test_tile() {
    test_is_tile_id_valid();
    test_is_tile_id_honors();
    test_is_tile_id_wind();
    test_is_tile_id_dragon();
    test_is_tile_id_man();
    test_is_tile_id_pin();
    test_is_tile_id_sou();
    test_is_tile_id_yaochu();
    test_is_tile_id_routou();
    test_get_tile_type();
    test_get_tile_number();
    test_gen_tiles_from_hands();
    test_tile_id_str();
    return true;
}
