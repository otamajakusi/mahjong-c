//
//
//
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "tile.h"

const uint8_t TILE_ID[] = {
     1, 2, 3, 4, 5, 6, 7, 8, 9, // man1~9
    11,12,13,14,15,16,17,18,19, // pin1~9
    21,22,23,24,25,26,27,28,29, // sou1~9
    31,32,33,34, // ton,nan,sha,pei
    41,42,43, // haku,hatsu,chun
};

static uint32_t TILE_ID_TO_INDEX[MAX_TILE_ID + 1];

// internal struct
typedef struct {
    uint32_t tile_num[sizeof(TILE_ID)];
} _Tiles;


static void dump_meld(const Meld *meld) {
    printf("meld(%d): %2d, %2d, %2d %2d\n", meld->len, meld->tile[0], meld->tile[1], meld->tile[2], meld->tile[3]);
}


static void generate_tile_id_to_index() {
    memset(TILE_ID_TO_INDEX, 0, sizeof(TILE_ID_TO_INDEX));
    for (int i = 0; i < sizeof(TILE_ID); i ++) {
        int idx = TILE_ID[i];
        assert(idx <= MAX_TILE_ID);
        TILE_ID_TO_INDEX[idx] = i;
    }
}

static uint32_t tile_id_to_index(uint8_t tile_id) {
    return TILE_ID_TO_INDEX[tile_id];
}


static int tile_cmp(const void *p1, const void *p2) {
    return *(const uint8_t*)p1 - *(const uint8_t*)p2;
}

static int is_valid_tile_id(uint8_t tile_id) {
    for (int i = 0; i < sizeof(TILE_ID); i ++) {
        if (tile_id == TILE_ID[i]) {
            return true;
        }
    }
    return false;
}

static int is_valid_hands(const Hands *hands) {
    for (int i = 0; i < hands->len; i ++) {
        if (!is_valid_tile_id(hands->tile[i])) {
            return false;
        }
    }
    return true;
}

// NOTE: tile_id should be checked before hand
static int is_meld_chi(const Meld *meld) {
    if (meld->len != 3) {
        return false;
    }
    Meld temp;
    memcpy(&temp, meld, sizeof(Meld));
    qsort(&temp.tile[0], temp.len, sizeof(temp.tile[0]), tile_cmp);
    if (temp.tile[0] >= 31) { // tile must be man, pin or sou. should not be winds or dragons.
        return false;
    }
    for (int i = 1; i < temp.len; i ++) {
        if (temp.tile[0] + i != temp.tile[i]) {
            return false;
        }
    }
    return true;
}

// NOTE: tile_id should be checked before hand
static int is_meld_pon(const Meld *meld) {
    if (meld->len != 3) {
        return false;
    }
    for (int i = 1; i < meld->len; i ++) {
        if (meld->tile[0] != meld->tile[i]) {
            return false;
        }
    }
    return true;
}

// NOTE: tile_id should be checked before hand
static int is_meld_kan(const Meld *meld) {
    if (meld->len != 4) {
        return false;
    }
    for (int i = 1; i < meld->len; i ++) {
        if (meld->tile[0] != meld->tile[i]) {
            return false;
        }
    }
    return true;
}

static int is_valid_meld(const Meld *meld) {
    if (meld->len != 3 && meld->len != 4) {
        fprintf(stderr, "invalid meld len %d\n", meld->len);
        return false;
    }
    for (int i = 0; i < meld->len; i ++) {
        if (!is_valid_tile_id(meld->tile[i])) {
            fprintf(stderr, "invalid meld tile %d\n", meld->tile[i]);
            return false;
        }
    }
    int is_chi, is_pon, is_kan;
    is_chi = is_meld_chi(meld);
    is_pon = is_meld_pon(meld);
    is_kan = is_meld_kan(meld);
    if (!is_chi && !is_pon && !is_kan) {
        fprintf(stderr, "neither chi, pon or kan\n");
        return false;
    }
    return true;
}

static int is_valid_melds(const Melds *melds) {
    for (int i = 0; i < melds->len; i ++) {
        if (!is_valid_meld(&melds->meld[i])) {
            return false;
        }
    }
    return true;
}

static int is_melds_and_win_tile_in_hands(const Hands *hands, const Melds *melds, uint8_t win_tile) {
    _Tiles tiles;
    memset(&tiles, 0, sizeof(_Tiles));
    for (int i = 0; i < hands->len; i ++) {
        uint32_t idx = tile_id_to_index(hands->tile[i]);
        tiles.tile_num[idx] ++;
    }
    for (int i = 0; i < melds->len; i ++) {
        const Meld *meld = &melds->meld[i];
        for (int j = 0; j < meld->len; j ++) {
            uint32_t idx = tile_id_to_index(meld->tile[j]);
            if (tiles.tile_num[idx] == 0) {
                fprintf(stderr, "tile in meld %d doesn't exists in hands\n", meld->tile[j]);
                return false;
            }
            tiles.tile_num[idx] --;
        }
    }
    {
        uint32_t idx = tile_id_to_index(win_tile);
        if (tiles.tile_num[idx] == 0) {
            fprintf(stderr, "win tile %d doesn't exists in hands\n", win_tile);
            return false;
        }
        tiles.tile_num[idx] --;
    }
    return true;
}

int32_t tile_get_score(Score *score, const Hands *hands, const Melds *melds, uint8_t win_tile, uint32_t is_ron) {
    if (!is_valid_hands(hands)) {
        return ERR_ILLEGAL_PARAM;
    }
    if (!is_valid_melds(melds)) {
        return ERR_ILLEGAL_PARAM;
    }
    if (!is_valid_tile_id(win_tile)) {
        return ERR_ILLEGAL_PARAM;
    }
    generate_tile_id_to_index();
    if (!is_melds_and_win_tile_in_hands(hands, melds, win_tile)) {
        return ERR_ILLEGAL_PARAM;
    }
    return 0;
}

