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
#include "hand.h"
#include "meld.h"
#include "element.h"

static bool is_melds_and_win_tile_in_tiles(const Tiles *tiles, const MJMelds *melds, MJTileId win_tile) {
    Tiles _tiles;
    memcpy(&_tiles, tiles, sizeof(Tiles));
    for (uint32_t i = 0; i < melds->len; i ++) {
        const MJMeld *meld = &melds->meld[i];
        for (uint32_t j = 0; j < meld->len; j ++) {
            MJTileId tile_id = meld->tile_id[j];
            if (tiles->tiles[tile_id] == 0) {
                fprintf(stderr, "tile in meld %d doesn't exists in hands\n", meld->tile_id[j]);
                return false;
            }
            _tiles.tiles[tile_id] --;
        }
    }
    if (_tiles.tiles[win_tile] == 0) {
        fprintf(stderr, "win tile %d doesn't exists in hands\n", win_tile);
        return false;
    }
    return true;
}

int32_t mj_get_score(
      MJScore *score,
      const MJHands *hands,
      const MJMelds *melds,
      MJTileId win_tile,
      bool ron,
      MJTileId player_wind,
      MJTileId round_wind) {
    (void)score;
    (void)ron;
    (void)player_wind;
    (void)round_wind;
    if (hands->len > MJ_MAX_HAND_LEN) {
        return MJ_ERR_NUM_TILES_LARGE;
    }
    if (hands->len < MJ_MIN_HAND_LEN) {
        return MJ_ERR_NUM_TILES_SHORT;
    }
    if (!is_valid_hands(hands)) {
        return MJ_ERR_ILLEGAL_PARAM;
    }
    if (!is_valid_melds(melds)) {
        return MJ_ERR_ILLEGAL_PARAM;
    }

    Tiles tiles;
    if (!gen_tiles_from_hands(&tiles, hands)) {
        return MJ_ERR_ILLEGAL_PARAM;
    }
    if (!is_melds_and_win_tile_in_tiles(&tiles, melds, win_tile)) {
        return MJ_ERR_ILLEGAL_PARAM;
    }

    Elements meld_elems;
    if (!gen_elements_from_melds(&meld_elems, melds)) {
        return MJ_ERR_ILLEGAL_PARAM;
    }
    return MJ_OK;
}