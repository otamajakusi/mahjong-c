#include <stdio.h>
#include <assert.h>

#include "test_tile.h"

static void test_is_tile_id_valid() {
    assert(is_tile_id_valid(MJ_M1));
    assert(is_tile_id_valid(MJ_DR));
    assert(!is_tile_id_valid(-1));
    assert(!is_tile_id_valid(MJ_DR+1));
}

static void test_is_tile_id_honors() {
    assert(is_tile_id_honors(MJ_WT));
    assert(is_tile_id_honors(MJ_WN));
    assert(is_tile_id_honors(MJ_WS));
    assert(is_tile_id_honors(MJ_WP));
    assert(is_tile_id_honors(MJ_DW));
    assert(is_tile_id_honors(MJ_DG));
    assert(is_tile_id_honors(MJ_DR));
    assert(!is_tile_id_honors(MJ_M1));
    assert(!is_tile_id_honors(MJ_S9));
}

static void test_is_tile_id_wind() {
    assert(is_tile_id_wind(MJ_WT));
    assert(is_tile_id_wind(MJ_WN));
    assert(is_tile_id_wind(MJ_WS));
    assert(is_tile_id_wind(MJ_WP));
    assert(!is_tile_id_wind(MJ_M1));
    assert(!is_tile_id_wind(MJ_S1));
    assert(!is_tile_id_wind(MJ_S9));
    assert(!is_tile_id_wind(MJ_DR));
}

static void test_is_tile_id_dragon() {
    assert(is_tile_id_dragon(MJ_DW));
    assert(is_tile_id_dragon(MJ_DG));
    assert(is_tile_id_dragon(MJ_DR));
    assert(!is_tile_id_dragon(MJ_DR+1));
    assert(!is_tile_id_dragon(MJ_WP));
    assert(!is_tile_id_dragon(MJ_M1));
    assert(!is_tile_id_dragon(MJ_S9));
}

static void test_is_tile_id_man() {
    assert(is_tile_id_man(MJ_M1));
    assert(is_tile_id_man(MJ_M2));
    assert(is_tile_id_man(MJ_M3));
    assert(is_tile_id_man(MJ_M4));
    assert(is_tile_id_man(MJ_M5));
    assert(is_tile_id_man(MJ_M6));
    assert(is_tile_id_man(MJ_M7));
    assert(is_tile_id_man(MJ_M8));
    assert(is_tile_id_man(MJ_M9));
    assert(!is_tile_id_man(MJ_M1-1));
    assert(!is_tile_id_man(MJ_P1));
    assert(!is_tile_id_man(MJ_WT));
    assert(!is_tile_id_man(MJ_DW));
    assert(!is_tile_id_man(MJ_DR));
}

static void test_is_tile_id_pin() {
    assert(is_tile_id_pin(MJ_P1));
    assert(is_tile_id_pin(MJ_P2));
    assert(is_tile_id_pin(MJ_P3));
    assert(is_tile_id_pin(MJ_P4));
    assert(is_tile_id_pin(MJ_P5));
    assert(is_tile_id_pin(MJ_P6));
    assert(is_tile_id_pin(MJ_P7));
    assert(is_tile_id_pin(MJ_P8));
    assert(is_tile_id_pin(MJ_P9));
    assert(!is_tile_id_pin(MJ_P1-1));
    assert(!is_tile_id_pin(MJ_M1));
    assert(!is_tile_id_pin(MJ_S1));
    assert(!is_tile_id_pin(MJ_WT));
    assert(!is_tile_id_pin(MJ_DW));
    assert(!is_tile_id_pin(MJ_DR));
}

static void test_is_tile_id_sou() {
    assert(is_tile_id_sou(MJ_S1));
    assert(is_tile_id_sou(MJ_S2));
    assert(is_tile_id_sou(MJ_S3));
    assert(is_tile_id_sou(MJ_S4));
    assert(is_tile_id_sou(MJ_S5));
    assert(is_tile_id_sou(MJ_S6));
    assert(is_tile_id_sou(MJ_S7));
    assert(is_tile_id_sou(MJ_S8));
    assert(is_tile_id_sou(MJ_S9));
    assert(!is_tile_id_sou(MJ_S1-1));
    assert(!is_tile_id_sou(MJ_M1));
    assert(!is_tile_id_sou(MJ_P1));
    assert(!is_tile_id_sou(MJ_WT));
    assert(!is_tile_id_sou(MJ_DW));
    assert(!is_tile_id_sou(MJ_DR));
}

/* 1,9字牌 */
static void test_is_tile_id_yaochu() {
    assert(is_tile_id_yaochu(MJ_M1));
    assert(is_tile_id_yaochu(MJ_M9));
    assert(is_tile_id_yaochu(MJ_P1));
    assert(is_tile_id_yaochu(MJ_P9));
    assert(is_tile_id_yaochu(MJ_S1));
    assert(is_tile_id_yaochu(MJ_S9));
    assert(is_tile_id_yaochu(MJ_WT));
    assert(is_tile_id_yaochu(MJ_WP));
    assert(is_tile_id_yaochu(MJ_DW));
    assert(is_tile_id_yaochu(MJ_DR));
    assert(!is_tile_id_yaochu(MJ_M2));
    assert(!is_tile_id_yaochu(MJ_M8));
    assert(!is_tile_id_yaochu(MJ_P2));
    assert(!is_tile_id_yaochu(MJ_P8));
    assert(!is_tile_id_yaochu(MJ_S2));
    assert(!is_tile_id_yaochu(MJ_S8));
}

/* 1,9牌 */
static void test_is_tile_id_routou() {
    assert(is_tile_id_routou(MJ_M1));
    assert(is_tile_id_routou(MJ_M9));
    assert(is_tile_id_routou(MJ_P1));
    assert(is_tile_id_routou(MJ_P9));
    assert(is_tile_id_routou(MJ_S1));
    assert(is_tile_id_routou(MJ_S9));
    assert(!is_tile_id_routou(MJ_WT));
    assert(!is_tile_id_routou(MJ_WP));
    assert(!is_tile_id_routou(MJ_DW));
    assert(!is_tile_id_routou(MJ_DR));
    assert(!is_tile_id_routou(MJ_M2));
    assert(!is_tile_id_routou(MJ_M8));
    assert(!is_tile_id_routou(MJ_P2));
    assert(!is_tile_id_routou(MJ_P8));
    assert(!is_tile_id_routou(MJ_S2));
    assert(!is_tile_id_routou(MJ_S8));
}


static void test_get_tile_type() {
    assert(get_tile_type(MJ_M1) == TILE_TYPE_MAN);
    assert(get_tile_type(MJ_M9) == TILE_TYPE_MAN);
    assert(get_tile_type(MJ_P1) == TILE_TYPE_PIN);
    assert(get_tile_type(MJ_P9) == TILE_TYPE_PIN);
    assert(get_tile_type(MJ_S1) == TILE_TYPE_SOU);
    assert(get_tile_type(MJ_S9) == TILE_TYPE_SOU);
    assert(get_tile_type(MJ_WT) == TILE_TYPE_WIND);
    assert(get_tile_type(MJ_WN) == TILE_TYPE_WIND);
    assert(get_tile_type(MJ_WS) == TILE_TYPE_WIND);
    assert(get_tile_type(MJ_WP) == TILE_TYPE_WIND);
    assert(get_tile_type(MJ_DW) == TILE_TYPE_DRAGON);
    assert(get_tile_type(MJ_DG) == TILE_TYPE_DRAGON);
    assert(get_tile_type(MJ_DR) == TILE_TYPE_DRAGON);
    assert(get_tile_type(-1) == TILE_TYPE_INVALID);
}

static void test_get_tile_number() {
    assert(get_tile_number(MJ_M1) == TILE_NUM_1);
    assert(get_tile_number(MJ_M9) == TILE_NUM_9);
    assert(get_tile_number(MJ_P2) == TILE_NUM_2);
    assert(get_tile_number(MJ_P8) == TILE_NUM_8);
    assert(get_tile_number(MJ_S3) == TILE_NUM_3);
    assert(get_tile_number(MJ_S7) == TILE_NUM_7);
    assert(get_tile_number(MJ_WT) == TILE_NUM_INVALID);
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
    MJHands hands = {{MJ_M1,MJ_M2,MJ_M3, MJ_WT,MJ_WT,MJ_WT, MJ_P7,MJ_P8,MJ_P9, MJ_S4,MJ_S5,MJ_S6, MJ_DW,MJ_DW}, 14};
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
    assert(strcmp(tile_id_str(MJ_M1), "m1") == 0);
    assert(strcmp(tile_id_str(MJ_M9), "m9") == 0);
    assert(strcmp(tile_id_str(MJ_P1), "p1") == 0);
    assert(strcmp(tile_id_str(MJ_P9), "p9") == 0);
    assert(strcmp(tile_id_str(MJ_S1), "s1") == 0);
    assert(strcmp(tile_id_str(MJ_S9), "s9") == 0);
    assert(strcmp(tile_id_str(MJ_WT), "wt") == 0);
    assert(strcmp(tile_id_str(MJ_WP), "wp") == 0);
    assert(strcmp(tile_id_str(MJ_DW), "dw") == 0);
    assert(strcmp(tile_id_str(MJ_DR), "dr") == 0);
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
