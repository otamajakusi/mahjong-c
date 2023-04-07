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

#pragma once

/*
 * [用語]
 * tile: 牌
 * element: 面子
 * meld: チー, ポン, カン(暗槓含む)
 * hand: 手牌(副露牌を含む)
 * pair: 雀頭
 * sequence: 順子
 * triplets: 刻子
 * fours: 槓子
 * concealed: 暗-; concealed-triplets 暗刻, concealed-fours 暗槓
 * melded: 明-; melded-triplets 明刻, melded-fours 明槓
 * honors tile: 字牌
 * 参照: http://crescent.s255.xrea.com/cabinet/others/mahjong/
 */

/*
 * prefix: mj, MJ
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif  // defined(__cplusplus)

#define MJ_OK 0
#define MJ_ERR_ILLEGAL_PARAM -1
#define MJ_ERR_NUM_TILES_SHORT -2
#define MJ_ERR_NUM_TILES_LARGE -3
#define MJ_ERR_AGARI_NOT_FOUND -4

#define MJ_ELEMENTS_LEN 4              // length of elements
#define MJ_PAIR_LEN 2                  // length of pairs
#define MJ_MAX_TILES_LEN_IN_ELEMENT 4  // max number of tiles in element
#define MJ_MIN_TILES_LEN_IN_ELEMENT 3  // min number of tiles in element
#define MJ_MAX_HAND_LEN (MJ_MAX_TILES_LEN_IN_ELEMENT * MJ_ELEMENTS_LEN + MJ_PAIR_LEN)
#define MJ_MIN_HAND_LEN (MJ_MIN_TILES_LEN_IN_ELEMENT * MJ_ELEMENTS_LEN + MJ_PAIR_LEN)

#define MJ_MAX_YAKU_NAME_LEN 2048

typedef enum {
  MJ_M1 = 0,
  MJ_M2,
  MJ_M3,
  MJ_M4,
  MJ_M5,
  MJ_M6,
  MJ_M7,
  MJ_M8,
  MJ_M9,
  MJ_P1,
  MJ_P2,
  MJ_P3,
  MJ_P4,
  MJ_P5,
  MJ_P6,
  MJ_P7,
  MJ_P8,
  MJ_P9,
  MJ_S1,
  MJ_S2,
  MJ_S3,
  MJ_S4,
  MJ_S5,
  MJ_S6,
  MJ_S7,
  MJ_S8,
  MJ_S9,
  MJ_WT,  // wind ton
  MJ_WN,  // wind nan
  MJ_WS,  // wind sha
  MJ_WP,  // wind pei
  MJ_DW,  // haku, dragon white
  MJ_DG,  // hatsu, dragon green
  MJ_DR,  // chun, dragon red
} MJTileId;

// NOTE: 暗槓も含む
typedef struct {
  MJTileId tile_id[MJ_MAX_TILES_LEN_IN_ELEMENT];
  uint32_t len;    // valid tile_id length, for sequence or triplets; 3, for fours; 4, should be set.
  bool concealed;  // set true if concealed-fours
  uint32_t type;   // internal use
} MJMeld;

typedef struct {
  MJMeld meld[MJ_ELEMENTS_LEN];
  uint32_t len;  // valid meld length
} MJMelds;

typedef struct {
  MJTileId tile_id[MJ_MAX_HAND_LEN];
  uint32_t len;  // valid tile_id length
} MJHands;

typedef struct {
  uint32_t han;
  uint32_t fu;
  char yaku_name[MJ_MAX_YAKU_NAME_LEN];
} MJBaseScore;

/*
 * return
 *   MJ_OK: success
 * params
 *   [out]
 *     score: calculated Score
 *   [in]
 *     hands: all tiles include melds and win_tile
 *     melds: list of meld
 *     win_tile: win_tile
 */
int32_t mj_get_score(MJBaseScore *score, const MJHands *hands, const MJMelds *melds, MJTileId win_tile, bool ron,
                     MJTileId player_wind, MJTileId round_wind);
#if defined(__cplusplus)
}
#endif  // defined(__cplusplus)
