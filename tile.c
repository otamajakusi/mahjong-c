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

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "tile.h"
#include "score.h"

#define DUMP_TILES  (0)

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

// 刻子のtile_idを最大MENTSU_LEN保存する. 保存サイズはlenに設定する.
typedef struct {
    uint8_t tile_ids[MENTSU_LEN];
    uint32_t len;
} Kotsu;

typedef struct {
    char str[128];
} MeldsStr;

static void str_melds(const Melds *melds, MeldsStr *str) {
    int n = 0;
    if (melds->len == 0) {
        str->str[0] = '\0';
        return;
    }
    for (int i = 0; i < melds->len; i ++) {
        const Meld *meld = &melds->meld[i];
        n += snprintf(&str->str[n], sizeof(MeldsStr) - n, "meld[%d]: ", i);
        for (int j = 0; j < meld->len; j ++) {
            n += snprintf(&str->str[n], sizeof(MeldsStr) - n, "%s ", tile_id_to_str[meld->tile_id[j]]);
        }
        n += snprintf(&str->str[n], sizeof(MeldsStr) - n, "\n");
    }
}

static void dump_melds(const Melds *melds) {
    MeldsStr str;
    str_melds(melds, &str);
    printf("%s", str.str);
}

#if DUMP_TILES
static void dump_tiles(const Tiles *tiles, int depth) {
    char indent[MENTSU_LEN * 3] = {0};
    for (int i = 0; i < depth * 3; i += 3) {
        strcpy(&indent[i], "   ");
    }
    printf("tiles: %s", indent);
    for (int i = 0; i < TILE_ID_LEN; i ++) {
        uint8_t tile_num = tiles->tiles[i];
        for (int n = 0; n < tile_num; n ++) {
            printf("%s ", tile_id_to_str[i]);
        }
    }
    printf("\n");
}
#else
#define dump_tiles(...)
#endif


static int tile_cmp(const void *p1, const void *p2) {
    return *(const uint8_t*)p1 - *(const uint8_t*)p2;
}

static void sort_meld(Meld *meld) {
    qsort(&meld->tile_id[0], meld->len, sizeof(meld->tile_id[0]), tile_cmp);
}

static void sort_melds(Melds *melds) {
    for (int i = 0; i < melds->len; i ++) {
        sort_meld(&melds->meld[i]);
    }
}

static int is_valid_tile_id(uint8_t tile_id) {
    return tile_id < TILE_ID_LEN;
}

static int is_valid_hands(const Hands *hands) {
    for (int i = 0; i < hands->len; i ++) {
        if (!is_valid_tile_id(hands->tile_id[i])) {
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
    sort_meld(&temp);
    int is_man = IS_MAN(temp.tile_id[0]);
    int is_pin = IS_PIN(temp.tile_id[0]);
    int is_sou = IS_SOU(temp.tile_id[0]);
    if (!is_man && !is_pin && !is_sou) {
        return false;
    }
    for (int i = 1; i < temp.len; i ++) {
        if (temp.tile_id[0] + i != temp.tile_id[i]) {
            return false;
        }
        if (is_man && !IS_MAN(temp.tile_id[i])) {
            fprintf(stderr, "illegal combination %d, %d\n", temp.tile_id[0], temp.tile_id[i]);
            return false;
        }
        if (is_pin && !IS_PIN(temp.tile_id[i])) {
            fprintf(stderr, "illegal combination %d, %d\n", temp.tile_id[0], temp.tile_id[i]);
            return false;
        }
        if (is_sou && !IS_SOU(temp.tile_id[i])) {
            fprintf(stderr, "illegal combination %d, %d\n", temp.tile_id[0], temp.tile_id[i]);
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
        if (meld->tile_id[0] != meld->tile_id[i]) {
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
        if (meld->tile_id[0] != meld->tile_id[i]) {
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
        if (!is_valid_tile_id(meld->tile_id[i])) {
            fprintf(stderr, "invalid meld tile %d\n", meld->tile_id[i]);
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
        uint32_t tile_id = hands->tile_id[i];
        tiles->tiles[tile_id] ++;
    }
    for (int i = 0; i < melds->len; i ++) {
        const Meld *meld = &melds->meld[i];
        for (int j = 0; j < meld->len; j ++) {
            uint32_t tile_id = meld->tile_id[j];
            if (tiles->tiles[tile_id] == 0) {
                fprintf(stderr, "tile in meld %d doesn't exists in hands\n", meld->tile_id[j]);
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


static int find_melds_as_shuntsu(const Tiles *tiles, int depth, Melds *tiles_melds) {
    Tiles _tiles;
    memcpy(&_tiles, tiles, sizeof(Tiles));
    dump_tiles(&_tiles, depth);
    for (int i = 0; i < TILE_ID_LEN; i ++) {
        uint8_t tile_num = _tiles.tiles[i];
        if (tile_num) {
            if (IS_WIND(i) || IS_DRAGON(i)) {
                return false;
            }
            if (i == MAN8 || i == MAN9 || i == PIN8 || i == PIN9 || i == SOU8 || i == SOU9) {
                return false;
            }
            if (_tiles.tiles[i + 1] == 0 || _tiles.tiles[i + 2] == 0) {
                return false;
            }
            _tiles.tiles[i] --;
            _tiles.tiles[i + 1] --;
            _tiles.tiles[i + 2] --;
            tiles_melds->meld[tiles_melds->len].tile_id[0] = i;
            tiles_melds->meld[tiles_melds->len].tile_id[1] = i + 1;
            tiles_melds->meld[tiles_melds->len].tile_id[2] = i + 2;
            tiles_melds->meld[tiles_melds->len].len = 3;
            tiles_melds->meld[tiles_melds->len].is_closed = 1;
            tiles_melds->len ++;

            int found = find_melds_as_shuntsu(&_tiles, depth + 1, tiles_melds);
            if (found) {
                return true;
            }
            tiles_melds->len --;
            _tiles.tiles[i] ++;
            _tiles.tiles[i + 1] ++;
            _tiles.tiles[i + 2] ++;
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
static void get_kotsu_candidates(Kotsu *kotsu, const Tiles *tiles) {
    memset(kotsu, 0, sizeof(Kotsu));
    for (int i = 0; i < TILE_ID_LEN; i ++) {
        uint8_t tile_num = tiles->tiles[i];
        if (tile_num >= 3) {
            assert(kotsu->len < MENTSU_LEN);
            kotsu->tile_ids[kotsu->len] = i;
            kotsu->len ++;
        }
    }
}

// NOTE: open_melds includes closed-kan
static int is_agari_with_kotsu(
        const Kotsu *kotsu,
        const Tiles *tiles,
        int head,
        const Melds *open_melds,
        const ScoreConfig *cfg) {
    Melds tiles_melds;
    int agari = 0;

    tiles_melds.len = 0;
    if (find_melds_as_shuntsu(tiles, 0, &tiles_melds)) {
        dump_melds(&tiles_melds);
        dump_melds(open_melds);
        calc_score(&tiles_melds, open_melds, head, cfg);
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
        //printf("kotsu %s is removed\n", tile_id_to_str[tile_id]);
        tiles_melds.len = 0;
        tiles_melds.meld[tiles_melds.len].tile_id[0] = tile_id;
        tiles_melds.meld[tiles_melds.len].tile_id[1] = tile_id;
        tiles_melds.meld[tiles_melds.len].tile_id[2] = tile_id;
        tiles_melds.meld[tiles_melds.len].len = 3;
        tiles_melds.meld[tiles_melds.len].is_closed = 1;
        tiles_melds.len ++;
        if (find_melds_as_shuntsu(&copy, 0, &tiles_melds)) {
            dump_melds(&tiles_melds);
            dump_melds(open_melds);
            calc_score(&tiles_melds, open_melds, head, cfg);
            agari ++;
        }
        copy.tiles[tile_id] += 3; // revert: remove kotsu
    }

    if (kotsu->len == 1) {
        return agari;
    }

    // remove all kotsu set
    tiles_melds.len = 0;
    for (int i = 0; i < kotsu->len; i ++) {
        uint8_t tile_id = kotsu->tile_ids[i];
        //printf("kotsu %s is removed\n", tile_id_to_str[tile_id]);
        copy.tiles[tile_id] -= 3;
        tiles_melds.meld[tiles_melds.len].tile_id[0] = tile_id;
        tiles_melds.meld[tiles_melds.len].tile_id[1] = tile_id;
        tiles_melds.meld[tiles_melds.len].tile_id[2] = tile_id;
        tiles_melds.meld[tiles_melds.len].len = 3;
        tiles_melds.meld[tiles_melds.len].is_closed = 1;
        tiles_melds.len ++;
    }
    if (find_melds_as_shuntsu(&copy, 0, &tiles_melds)) {
        dump_melds(&tiles_melds);
        dump_melds(open_melds);
        calc_score(&tiles_melds, open_melds, head, cfg);
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
static int is_agari(const Tiles *tiles, const Melds *melds, const ScoreConfig *cfg) {
    // TODO: need special agari type. e.g. chitoitsu, kokushi
    Tiles _tiles;
    memcpy(&_tiles, tiles, sizeof(Tiles));
    for (int i = 0; i < TILE_ID_LEN; i ++) {
        if (tiles->tiles[i] >= 2) {
            _tiles.tiles[i] -= 2; // remove head
            Kotsu kotsu;
            get_kotsu_candidates(&kotsu, &_tiles);
            int agari;
            agari = is_agari_with_kotsu(&kotsu, &_tiles, i, melds, cfg);
            if (agari) {
                printf("agari: head: %s \n", tile_id_to_str[i]);
                dump_tiles(tiles, 0);
            }
            _tiles.tiles[i] += 2; // revert: remove head
        }
    }
    return true;
}

int32_t tile_get_score(
        Score *score,
        const Hands *hands,
        const Melds *melds,
        uint8_t win_tile,
        uint32_t is_ron,
        uint32_t player_wind,
        uint32_t round_wind) {
    if (!is_valid_hands(hands)) {
        return ERR_ILLEGAL_PARAM;
    }
    if (!is_valid_melds(melds)) {
        return ERR_ILLEGAL_PARAM;
    }
    Melds meld_sorted;
    memcpy(&meld_sorted, melds, sizeof(Melds));
    sort_melds(&meld_sorted);

    if (!is_valid_tile_id(win_tile)) {
        return ERR_ILLEGAL_PARAM;
    }
    Tiles tiles;
    if (!is_melds_and_win_tile_in_hands(&tiles, hands, melds, win_tile)) {
        return ERR_ILLEGAL_PARAM;
    }

    // 

    ScoreConfig cfg = {win_tile, is_ron, player_wind, round_wind};
    is_agari(&tiles, melds, &cfg);
    return 0;
}
#endif // if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mahjong.h"
#include "tile.h"

bool is_tile_id_valid(MJTileId tile_id) {
    return (tile_id >= MJ_M1 && tile_id <= MJ_DR);
}

bool is_tile_id_honors(MJTileId tile_id) {
    return (tile_id >= MJ_WT && tile_id <= MJ_DR);
}

bool is_tile_id_wind(MJTileId tile_id) {
    return (tile_id >= MJ_WT && tile_id <= MJ_WP);
}

bool is_tile_id_dragon(MJTileId tile_id) {
    return (tile_id >= MJ_DW && tile_id <= MJ_DR);
}

bool is_tile_id_man(MJTileId tile_id) {
    return (tile_id >= MJ_M1 && tile_id <= MJ_M9);
}

bool is_tile_id_pin(MJTileId tile_id) {
    return (tile_id >= MJ_P1 && tile_id <= MJ_P9);
}

bool is_tile_id_sou(MJTileId tile_id) {
    return (tile_id >= MJ_S1 && tile_id <= MJ_S9);
}

uint32_t get_tile_type(MJTileId tile_id) {
    if (0) {
    } else if (is_tile_id_man(tile_id)) {
        return TILE_TYPE_MAN;
    } else if (is_tile_id_pin(tile_id)) {
        return TILE_TYPE_PIN;
    } else if (is_tile_id_sou(tile_id)) {
        return TILE_TYPE_SOU;
    } else if (is_tile_id_wind(tile_id)) {
        return TILE_TYPE_WIND;
    } else if (is_tile_id_dragon(tile_id)) {
        return TILE_TYPE_DRAGON;
    }
    return TILE_TYPE_INVALID;
}
