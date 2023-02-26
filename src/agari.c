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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mahjong.h"
#include "tile.h"
#include "element.h"
#include "agari.h"

/*
 * [アガリ判定について]
 * 手牌(自摸を含み、副露牌、暗槓子を含まない)を以下の順で処理をする
 * なお国士無双と七対子はこの判定処理で判定できないので特別処理が必要
 *
 * 雀頭候補を順番に抜き残りの面子に以下を実施する
 * 1. 刻子候補(3枚組)が0の場合
 *    -> すべて順子で構成されればアガリ.
 *
 * 2. 刻子候補(3枚組)が1つの場合
 *    -> 刻子候補を抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *
 * 3. 刻子候補(3枚組)が2つ(A,B)の場合
 *    -> 刻子候補をAを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *    -> 刻子候補をBを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *    -> 刻子候補をAとBを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *
 * 4. 刻子候補(3枚組)が3つ(A,B,C)の場合
 *    -> 刻子候補をAを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *    -> 刻子候補をBを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *    -> 刻子候補をCを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *    -> 刻子候補をA,B,Cを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *
 * 5. 刻子候補(3枚組)が4つ(A,B,C,D)の場合
 *    -> 刻子候補をAを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *    -> 刻子候補をBを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *    -> 刻子候補をCを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *    -> 刻子候補をDを抜く
 *    -> 残りすべて順子で構成されればアガリ.
 *
 * 考え方
 * 2. 刻子候補(3枚組)が1つの場合
 *    123334455 の組で333は刻子にならないので刻子を抜く前にすべて順子になるか確認する
 * 3. 刻子候補(3枚組)が2つ(A,B)の場合
 *    233334445566 と 333444567567 の場合を考える. 333(=A), 444(=B)として
 *    Aを抜いて 234445566 はすべて順子なのでアガリ
 *    Aを抜いて 444567567. Bを抜いて 567567 はすべて順子なのでアガリ
 *    刻子1組を抜いて残りがすべて順子になる場合と刻子2組を抜いて残りがすべて順子になる場合がある
 * 4. 刻子候補(3枚組)が3つ(A,B,C)の場合
 *    333(=A),444(=B),555(=C),366で考える
 *    Aを抜いて残りがすべて順子の組み合わせ -> ある(344455566)
 *    ABを抜いて残りがすべて順子の組み合わせ -> ない. 刻子を2組抜いて更に3枚の同じ配を順子構成にするには全部で3組必要だが2組しか無い
 *    -> 2組刻子を抜く場合は考えなくてよい
 *    ABCを抜いて残りがすべて順子の組み合わせ -> ある (333444555789など)
 * 5. 刻子候補(3枚組)が4つ(A,B,C,D)の場合
 *    333(=A),444(=B),555(=C),666(=D)で考える
 *    Aを抜いて, BCDの順子の組み合わせは成り立つ. 同様にDを抜いてABCの順子の組み合わせは成り立つ
 *    AB,ACなど2組を抜いて、順子かつ刻子が2組の状態は存在しない -> 2組を抜くことは考えなくて良い
 *    ABC,ACDなど3組を抜いて、順子かつ刻子が1組の状態は存在しない -> 3組を抜くことは考えなくて良い
 *    (ABCDの4組を抜いた場合残りは0)
 *
 * 参考: https://www.engineer-log.com/entry/2018/06/14/mahjong-algorithm
 */

/* save triplets tile id to tile_id[n]. e.g. if there is two triplets, 2 should be set for len */
typedef struct {
    MJTileId tile_id[MJ_ELEMENTS_LEN];
    uint32_t len;
} _Triplets;

static void gen_triplets_candidates(_Triplets *triplets, const Tiles *tiles) {
    memset(triplets, 0, sizeof(_Triplets));
    for (uint32_t i = 0; i <= MJ_DR; i ++) {
        uint8_t tile_num = tiles->tiles[i];
        if (tile_num >= 3) { // pickup triplets candidate from tiles
            assert(triplets->len < MJ_ELEMENTS_LEN);
            triplets->tile_id[triplets->len] = i;
            triplets->len ++;
        }
    }
}

/*
 * tilesがすべて順子で成立するか確認する.
 * 成立した面子はelemsに追加される.
 */
static uint32_t find_elements_as_sequence(const Tiles *tiles, Elements *elems) {
    Tiles _tiles;
    memcpy(&_tiles, tiles, sizeof(Tiles));
    for (int i = 0; i <= MJ_DR; i ++) {
        uint8_t tile_num = _tiles.tiles[i];
        if (tile_num) {
            if (is_tile_id_honors(i)) {
                return false;
            }
            uint32_t number = get_tile_number(i);
            if (number == 7 || number == 8) { /* in case of M8, M9, P8, P9, S8, S9 */
                return false;
            }

            /* next and next of next do not exist */
            if (_tiles.tiles[i + 1] == 0 || _tiles.tiles[i + 2] == 0) {
                return false;
            }
            _tiles.tiles[i] --;
            _tiles.tiles[i + 1] --;
            _tiles.tiles[i + 2] --;
            assert(elems->len < MJ_ELEMENTS_LEN);
            elems->meld[elems->len].tile_id[0] = i;
            elems->meld[elems->len].tile_id[1] = i + 1;
            elems->meld[elems->len].tile_id[2] = i + 2;
            elems->meld[elems->len].len = 3;
            elems->meld[elems->len].concealed = true;
            elems->meld[elems->len].type = ELEM_TYPE_SEQUENCE;
            elems->len ++;

            uint32_t found = find_elements_as_sequence(&_tiles, elems);
            if (found) {
                return true;
            }
            elems->len --;
            _tiles.tiles[i] ++;
            _tiles.tiles[i + 1] ++;
            _tiles.tiles[i + 2] ++;
        }
    }
    return true;
}

/* すべてが順子で構成されるか */
static uint32_t find_agari_sequence(
        const Tiles *tiles,
        const Elements *melded_elems,
        MJTileId pair_tile,
        AgariCallbackElements *cb_elements,
        void *cbarg) {
    Elements elems;
    memset(&elems, 0, sizeof(Elements));

    uint32_t agari = 0;
    if (find_elements_as_sequence(tiles, &elems)) {
        cb_elements(&elems, melded_elems, pair_tile, cbarg);
        agari ++;
    }
    return agari;
}

/* 刻子候補を1つ取り出して残りが順子で構成されるか */
static uint32_t find_agari_sequence_after_remove_one_triplets(
        const Tiles *tiles,
        const _Triplets *triplets,
        const Elements *melded_elems,
        MJTileId pair_tile,
        AgariCallbackElements *cb_elements,
        void *cbarg) {
    Elements elems;
    Tiles _tiles;
    uint32_t agari = 0;

    memset(&elems, 0, sizeof(Elements));
    memcpy(&_tiles, tiles, sizeof(Tiles));
    // remove each kotsu set
    for (uint32_t i = 0; i < triplets->len; i ++) {
        MJTileId tile_id = triplets->tile_id[i];
        _tiles.tiles[tile_id] -= 3; // remove triplets
        elems.len = 1;
        elems.meld[0].tile_id[0] = tile_id;
        elems.meld[0].tile_id[1] = tile_id;
        elems.meld[0].tile_id[2] = tile_id;
        elems.meld[0].len = 3;
        elems.meld[0].concealed = true;
        elems.meld[0].type = ELEM_TYPE_TRIPLETS;
        if (find_elements_as_sequence(&_tiles, &elems)) {
            cb_elements(&elems, melded_elems, pair_tile, cbarg);
            agari ++;
        }
        _tiles.tiles[tile_id] += 3; // revert: remove triplets
    }
    return agari;
}

/* 刻子候補をすべて取り出して残りが順子で構成されるか */
static uint32_t find_agari_sequence_after_remove_all_triplets(
        const Tiles *tiles,
        const _Triplets *triplets,
        const Elements *melded_elems,
        MJTileId pair_tile,
        AgariCallbackElements *cb_elements,
        void *cbarg) {
    Elements elems;
    Tiles _tiles;
    uint32_t agari = 0;

    memset(&elems, 0, sizeof(Elements));
    memcpy(&_tiles, tiles, sizeof(Tiles));

    /* remove all triplets from tiles */
    for (uint32_t i = 0; i < triplets->len; i ++) {
        MJTileId tile_id = triplets->tile_id[i];
        _tiles.tiles[tile_id] -= 3; // remove triplets
        assert(elems.len < MJ_ELEMENTS_LEN);
        elems.meld[elems.len].tile_id[0] = tile_id;
        elems.meld[elems.len].tile_id[1] = tile_id;
        elems.meld[elems.len].tile_id[2] = tile_id;
        elems.meld[elems.len].len = 3;
        elems.meld[elems.len].concealed = true;
        elems.meld[elems.len].type = ELEM_TYPE_TRIPLETS;
        elems.len ++;
    }
    if (find_elements_as_sequence(&_tiles, &elems)) {
        cb_elements(&elems, melded_elems, pair_tile, cbarg);
        agari ++;
    }
    return agari;
}

/* NOTE: melded_elems includes concealed-fours */
static uint32_t find_agari_with_triplets_candidates(
        const _Triplets *triplets,
        const Tiles *concealed_tiles,
        const Elements *melded_elems,
        MJTileId pair_tile,
        AgariCallbackElements *cb_elements,
        void *cbarg) {
    uint32_t agari = 0;

    agari += find_agari_sequence(concealed_tiles, melded_elems, pair_tile, cb_elements, cbarg);

    if (triplets->len == 0) {
        return agari;
    }

    agari += find_agari_sequence_after_remove_one_triplets(concealed_tiles, triplets, melded_elems, pair_tile, cb_elements, cbarg);

    if (triplets->len == 1) {
        return agari;
    }

    agari += find_agari_sequence_after_remove_all_triplets(concealed_tiles, triplets, melded_elems, pair_tile, cb_elements, cbarg);

    return agari;
}

/*
 * tiles: concealed
 * melds: melded (includes an-kan)
 */
uint32_t find_agari(
    const Tiles *concealed_tiles,
    const Elements *melded_elems,
    AgariCallbackTiles *cb_tiles,
    AgariCallbackElements *cb_elements,
    void *cbarg) {

    uint32_t found;
    uint32_t agari = 0;
    if (melded_elems->len == 0) {
        found = (uint32_t)cb_tiles(concealed_tiles, cbarg);
        if (found) {
            agari ++;
        }
    }

    Tiles _concealed_tiles;
    memcpy(&_concealed_tiles, concealed_tiles, sizeof(Tiles));

    for (uint32_t i = 0; i <= MJ_DR; i ++) {
        if (concealed_tiles->tiles[i] >= MJ_PAIR_LEN) {
            _concealed_tiles.tiles[i] -= MJ_PAIR_LEN; // remove pair from tiles
            _Triplets triplets;
            gen_triplets_candidates(&triplets, &_concealed_tiles);
            found = find_agari_with_triplets_candidates(&triplets, &_concealed_tiles, melded_elems, i, cb_elements, cbarg);
            agari += found;
            if (found) {
                fprintf(stderr, "agari %d: pair: %s\n", agari, tile_id_str(i));
            }
            _concealed_tiles.tiles[i] += 2; // revert: remove pair from tiles
        }
    }
    return agari;
}
