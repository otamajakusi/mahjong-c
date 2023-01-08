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

static const char tile_id_to_str[][3] = {
    "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8", "m9",
    "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9",
    "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9",
    "wt", "wn", "ws", "wp",
    "dw", "dg", "dr",
};

// internal struct
typedef struct {
    uint32_t tiles[TILE_ID_LEN];
} Tiles;

typedef struct {
    uint32_t tile_ids[MENTSU_LEN];
    uint32_t len;
} KotsuCandidates;


static void dump_meld(const Meld *meld) {
    printf("meld(%d): %2d, %2d, %2d %2d\n", meld->len, meld->tile[0], meld->tile[1], meld->tile[2], meld->tile[3]);
}

static void dump_tiles(const Tiles *tiles) {
    printf("tiles: ");
    for (int i = 0; i < TILE_ID_LEN; i ++) {
        uint8_t tile_num = tiles->tiles[i];
        for (int n = 0; n < tile_num; n ++) {
            printf("%s ", tile_id_to_str[i]);
        }
    }
    printf("\n");
}


static int tile_cmp(const void *p1, const void *p2) {
    return *(const uint8_t*)p1 - *(const uint8_t*)p2;
}

static int is_valid_tile_id(uint8_t tile_id) {
    return tile_id < TILE_ID_LEN;
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
    int is_man = IS_MAN(temp.tile[0]);
    int is_pin = IS_PIN(temp.tile[0]);
    int is_sou = IS_SOU(temp.tile[0]);
    if (!is_man && !is_pin && !is_sou) {
        return false;
    }
    for (int i = 1; i < temp.len; i ++) {
        if (temp.tile[0] + i != temp.tile[i]) {
            return false;
        }
        if (is_man && !IS_MAN(temp.tile[i])) {
            fprintf(stderr, "illegal combination %d, %d\n", temp.tile[0], temp.tile[i]);
            return false;
        }
        if (is_pin && !IS_PIN(temp.tile[i])) {
            fprintf(stderr, "illegal combination %d, %d\n", temp.tile[0], temp.tile[i]);
            return false;
        }
        if (is_sou && !IS_SOU(temp.tile[i])) {
            fprintf(stderr, "illegal combination %d, %d\n", temp.tile[0], temp.tile[i]);
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

// this function checks if melds and win_tile are in hands
// and returns tiles removed melds from hands.
static int is_melds_and_win_tile_in_hands(Tiles *tiles, const Hands *hands, const Melds *melds, uint8_t win_tile) {
    memset(tiles, 0, sizeof(Tiles));
    for (int i = 0; i < hands->len; i ++) {
        uint32_t tile_id = hands->tile[i];
        tiles->tiles[tile_id] ++;
    }
    for (int i = 0; i < melds->len; i ++) {
        const Meld *meld = &melds->meld[i];
        for (int j = 0; j < meld->len; j ++) {
            uint32_t tile_id = meld->tile[j];
            if (tiles->tiles[tile_id] == 0) {
                fprintf(stderr, "tile in meld %d doesn't exists in hands\n", meld->tile[j]);
                return false;
            }
            tiles->tiles[tile_id] --;
        }
    }
    if (tiles->tiles[win_tile] == 0) {
        fprintf(stderr, "win tile %d doesn't exists in hands\n", win_tile);
        return false;
    }
    return true;
}


static int find_melds_as_shuntsu(const Tiles *tiles) {
    Tiles copy;
    memcpy(&copy, tiles, sizeof(Tiles));
    dump_tiles(&copy);
    for (int i = 0; i < TILE_ID_LEN; i ++) {
        uint8_t tile_num = copy.tiles[i];
        if (tile_num) {
            if (IS_WIND(i) || IS_DRAGON(i)) {
                return false;
            }
            if (i == MAN8 || i == MAN9 || i == PIN8 || i == PIN9 || i == SOU8 || i == SOU9) {
                return false;
            }
            if (copy.tiles[i + 1] == 0 || copy.tiles[i + 2] == 0) {
                return false;
            }
            copy.tiles[i] --;
            copy.tiles[i + 1] --;
            copy.tiles[i + 2] --;
            int found = find_melds_as_shuntsu(&copy);
            if (found) {
                return true;
            }
            copy.tiles[i] ++;
            copy.tiles[i + 1] ++;
            copy.tiles[i + 2] ++;
        }
    }
    return true;
}

// 刻子の組み合わせでループ
// 刻子候補が2組A,B有った場合, 0組, Aのみ, Bのみ, A,B組でループさせ残りを順子で取り出す
// https://www.engineer-log.com/entry/2018/06/14/mahjong-algorithm
//
// 刻子が1(A)の場合は、A, の組み合わせ
// -> 問題: 雀頭除いたあと, 刻子が1枚で残りがすべて順子(同じ牌が2枚以下)の場合, 刻子は常に刻子になるか?(順子要素にならないか?)
//    333があって、これがすべて順子要素になるためには,
//    123334455 というパターンがあるので常に刻子になるとは限らない.
// 刻子が2(A,B)の場合は、A, B, AB, の組み合わせ
// -> 23333444556688, あるいは 333444567567 の組み合わせを考える. 333(=A), 444(=B)として
//    A or Bを抜いて残りがすべて順子で構成されることがある
//    ABを抜いて残りがすべて順子で構成されることがある
//    -> 1枚抜く場合, 2枚の組を抜く場合の両方を実施する必要あり
// 刻子が3(A,B,C)の場合は、A, B, C, AB, AC, BC, ABC, の組み合わせ
// -> 333(=A),444(=B),555(=C),366で考える
//    Aを抜いて残りがすべて順子の組み合わせ -> ある
//    ABを抜いて残りがすべて順子の組み合わせ -> できない. 3枚の同じ配を順子構成にするには全部で3組必要だが2組しか無い
//    ABCを抜いて残りがすべて順子の組み合わせ -> ある
// 刻子が4(A,B,C,D)の場合は、A, B, C, D, AB, AC, AD, BC, BD, ABC, ABD, BCD, ABCD, の組み合わせ
// -> 333(=A),444(=B),555(=C),666(=D)で考える
//    Aを抜いて, BCDの順子の組み合わせは成り立つ. 同様にDを抜いてABCの順子の組み合わせは成り立つ
//    AB,ACなど2組を抜いて、順子かつ刻子が2組の状態は存在しない -> 2組を抜くことは考えなくて良い
//    ABC,ACDなど3組を抜いて、順子かつ刻子が1組の状態は存在しない -> 3組を抜くことは考えなくて良い
//    ABCDの4組を抜いた場合残りは0なので順子は残らない -> 4組を抜くことは特別に処理してもよい
// で刻子を抜いて順子を探す
//
// TODO: 333(=A),444(=B),555(=C),345の場合は刻子が3つにカウントされるけどOK?
//
// まとめ
// 刻子候補が0つの場合:
// - すべて順子で構成されるか調べる
// 刻子候補が1つ(A)の場合: 
// - すべて順子で構成されるか調べる
// - Aを抜いて残りがすべて順子で構成されるか調べる
// 刻子候補が2つ(A,B)の場合: 
// - すべて順子で構成されるか調べる
// - Aを抜いて残りがすべて順子で構成されるか調べる
// - Bを抜いて残りがすべて順子で構成されるか調べる
// - AとBを抜いて残りがすべて順子で構成されるか調べる
// 刻子候補が3つ(A,B,C)の場合: 
// - すべて順子で構成されるか調べる
// - Aを抜いて残りがすべて順子で構成されるか調べる
// - Bを抜いて残りがすべて順子で構成されるか調べる
// - Cを抜いて残りがすべて順子で構成されるか調べる
// - AとBとCを抜いて残りがすべて順子で構成されるか調べる
// 刻子候補が4つ(A,B,C,D)の場合: 
// - Aを抜いて残りがすべて順子で構成されるか調べる
// - Bを抜いて残りがすべて順子で構成されるか調べる
// - Cを抜いて残りがすべて順子で構成されるか調べる
// - Dを抜いて残りがすべて順子で構成されるか調べる
// - A,B,C,Dを抜いて残りが0なのであがり
//
static void get_kotsu_candidates(KotsuCandidates *kotsu, const Tiles *tiles) {
    memset(kotsu, 0, sizeof(KotsuCandidates));
    for (int i = 0; i < TILE_ID_LEN; i ++) {
        uint8_t tile_num = tiles->tiles[i];
        if (tile_num >= 3) {
            assert(kotsu->len < MENTSU_LEN);
            kotsu->tile_ids[kotsu->len] = i;
            kotsu->len ++;
        }
    }
}

static int is_agari_with_kotsu(const KotsuCandidates *kotsu, const Tiles *tiles) {
    int agari = 0;
    if (find_melds_as_shuntsu(tiles)) {
        agari ++;
    }

    if (kotsu->len == 0) {
        return agari;
    }

    Tiles copy;
    memcpy(&copy, tiles, sizeof(Tiles));

    // remove each kotsu set
    for (int i = 0; i < kotsu->len; i ++) {
        uint8_t tile_id = kotsu->tile_ids[i];
        copy.tiles[tile_id] -= 3; // remove kotsu
        printf("kotsu %s is removed\n", tile_id_to_str[tile_id]);
        if (find_melds_as_shuntsu(&copy)) {
            agari ++;
        }
        copy.tiles[tile_id] += 3; // revert: remove kotsu
    }
    // remove all kotsu set
    for (int i = 0; i < kotsu->len; i ++) {
        uint8_t tile_id = kotsu->tile_ids[i];
        printf("kotsu %s is removed\n", tile_id_to_str[tile_id]);
        copy.tiles[tile_id] -= 3;
    }
    if (find_melds_as_shuntsu(&copy)) {
        agari ++;
    }
    // revert: remove all kotsu set
    /* we don't need this operation since the `copy` is not used anymore.
    for (int i = 0; i < kotsu->len; i ++) {
        uint8_t tile_id = kotsu->tile_ids[i];
        copy.tiles[tile_id] += 3;
    }
    */
    return agari;
}

// make sure tiles doesn't contain melds's set
static int is_agari(const Tiles *tiles, const Melds *melds) {
    // TODO: need special agari type. e.g. chitoitsu, kokushi
    dump_tiles(tiles);
    Tiles copy;
    memcpy(&copy, tiles, sizeof(Tiles));
    for (int i = 0; i < TILE_ID_LEN; i ++) {
        if (tiles->tiles[i] >= 2) {
            copy.tiles[i] -= 2; // remove head
            KotsuCandidates kotsu;
            get_kotsu_candidates(&kotsu, &copy);
            int agari;
            printf("kotsu len %d, head %s\n", kotsu.len, tile_id_to_str[i]);
            agari = is_agari_with_kotsu(&kotsu, &copy);
            if (agari) {
                printf("agari: head: %s ", tile_id_to_str[i]);
                dump_tiles(tiles);
            }
            copy.tiles[i] += 2; // revert: remove head
        }
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
    Tiles tiles;
    if (!is_melds_and_win_tile_in_hands(&tiles, hands, melds, win_tile)) {
        return ERR_ILLEGAL_PARAM;
    }

    // 

    is_agari(&tiles, NULL);
    return 0;
}

