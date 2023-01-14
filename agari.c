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
#include "score.h"

/* save triplets tile id to tile_id[n]. if there is two triplets, 2 should be set for len */
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
 * 成立した面子はelemsに作成される.
 */
static int find_elements_as_sequence(const Tiles *tiles, Elements *elems) {
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
            elems->meld[elems->len].tile_id[0] = i;
            elems->meld[elems->len].tile_id[1] = i + 1;
            elems->meld[elems->len].tile_id[2] = i + 2;
            elems->meld[elems->len].len = 3;
            elems->meld[elems->len].concealed = 1;
            elems->len ++;

            int found = find_elements_as_sequence(&_tiles, elems);
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
static int find_agari_sequence(const Tiles *tiles, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    Elements elems;
    memset(&elems, 0, sizeof(Elements));

    int agari = 0;
    if (find_elements_as_sequence(tiles, &elems)) {
        calc_score(&elems, melded_elems, pair_tile, cfg);
        agari ++;
    }
    return agari;
}

/* 刻子候補を1つ取り出して残りが順子で構成されるか */
static int find_agari_sequence_after_remove_one_triplets(const Tiles *tiles, const _Triplets *triplets, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    Elements elems;
    Tiles _tiles;
    int agari = 0;

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
        if (find_elements_as_sequence(&_tiles, &elems)) {
            calc_score(&elems, melded_elems, pair_tile, cfg);
            agari ++;
        }
        _tiles.tiles[tile_id] += 3; // revert: remove triplets
    }
    return agari;
}

/* 刻子候補をすべて取り出して残りが順子で構成されるか */
static int find_agari_sequence_after_remove_all_triplets(const Tiles *tiles, const _Triplets *triplets, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    Elements elems;
    Tiles _tiles;
    int agari = 0;

    memset(&elems, 0, sizeof(Elements));
    memcpy(&_tiles, tiles, sizeof(Tiles));

    /* remove all triplets from tiles */
    for (uint32_t i = 0; i < triplets->len; i ++) {
        MJTileId tile_id = triplets->tile_id[i];
        _tiles.tiles[tile_id] -= 3; // remove triplets
        elems.meld[elems.len].tile_id[0] = tile_id;
        elems.meld[elems.len].tile_id[1] = tile_id;
        elems.meld[elems.len].tile_id[2] = tile_id;
        elems.meld[elems.len].len = 3;
        elems.meld[elems.len].concealed = true;
        elems.len ++;
    }
    if (find_elements_as_sequence(&_tiles, &elems)) {
        calc_score(&elems, melded_elems, pair_tile, cfg);
        agari ++;
    }
    return agari;
}

/* NOTE: melded_elems includes concealed-fours */
static int find_agari_with_triplets_candidates(
        const _Triplets *triplets,
        const Tiles *concealed_tiles,
        const Elements *melded_elems,
        MJTileId pair_tile,
        ScoreConfig *cfg) {
    int agari = 0;

    agari += find_agari_sequence(concealed_tiles, melded_elems, pair_tile, cfg);

    if (triplets->len == 0) {
        return agari;
    }

    agari += find_agari_sequence_after_remove_one_triplets(concealed_tiles, triplets, melded_elems, pair_tile, cfg);

    if (triplets->len == 1) {
        return agari;
    }

    agari += find_agari_sequence_after_remove_all_triplets(concealed_tiles, triplets, melded_elems, pair_tile, cfg);

    return agari;
}

/*
 * tiles: concealed
 * melds: melded (includes an-kan)
 */
bool find_agari(
    const Tiles *concealed_tiles,
    const Elements *melded_elems,
    MJTileId win_tile,
    bool ron,
    const MJTileId player_wind,
    const MJTileId round_wind) {
    ScoreConfig cfg = {win_tile, ron, player_wind, round_wind};

    if (melded_elems->len == 0) {
        calc_score_concealed_hands(concealed_tiles, ron);
    }

    Tiles _concealed_tiles;
    memcpy(&_concealed_tiles, concealed_tiles, sizeof(Tiles));

    for (uint32_t i = 0; i <= MJ_DR; i ++) {
        if (concealed_tiles->tiles[i] >= MJ_PAIR_LEN) {
            _concealed_tiles.tiles[i] -= MJ_PAIR_LEN; // remove pair from tiles
            _Triplets triplets;
            gen_triplets_candidates(&triplets, &_concealed_tiles);
            int agari;
            agari = find_agari_with_triplets_candidates(&triplets, &_concealed_tiles, melded_elems, i, &cfg);
            if (agari) {
                printf("agari: pair: %s\n", tile_id_str(i));
            }
            _concealed_tiles.tiles[i] += 2; // revert: remove pair from tiles
        }
    }
    return true;
}
