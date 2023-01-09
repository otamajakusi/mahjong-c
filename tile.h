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

#include <stdint.h>
#include <stdbool.h>

#define OK                  0
#define ERR_ILLEGAL_PARAM   -1
#define ERR_NUM_TILES_SHORT -2
#define ERR_NUM_TILES_LARGE -3

#define MENTSU_LEN          4 // mentsu len
#define MAX_MELD_LEN        4 // kan len
#define MAX_HAND_LEN        (MAX_MELD_LEN * MENTSU_LEN + 2) // 4-kans and 1-head
#define MAX_YAKU_NAME_LEN   64

typedef enum {
  INV = -1,
  MAN1 = 0,
  MAN2,
  MAN3,
  MAN4,
  MAN5,
  MAN6,
  MAN7,
  MAN8,
  MAN9,
  PIN1, // 9
  PIN2,
  PIN3,
  PIN4,
  PIN5,
  PIN6,
  PIN7,
  PIN8,
  PIN9,
  SOU1, // 18,
  SOU2,
  SOU3,
  SOU4,
  SOU5,
  SOU6,
  SOU7,
  SOU8,
  SOU9,
  TON, // 27,
  NAN,
  SHA,
  PEI,
  HAKU, // 31,
  HATSU,
  CHUN,
  TILE_ID_LEN, // 34
} TileId;


#define IS_MAN(tile_id)   ((tile_id) >= MAN1 && (tile_id) <= MAN9)
#define IS_PIN(tile_id)   ((tile_id) >= PIN1 && (tile_id) <= PIN9)
#define IS_SOU(tile_id)   ((tile_id) >= SOU1 && (tile_id) <= SOU9)
#define IS_WIND(tile_id)   ((tile_id) >= TON && (tile_id) <= PEI)
#define IS_DRAGON(tile_id)   ((tile_id) >= HAKU && (tile_id) <= CHUN)
#define IS_ONE(tile_id)   \
  ((tile_id) == MAN1 || \
   (tile_id) == PIN1 || \
   (tile_id) == SOU1)
#define IS_NINE(tile_id)   \
  ((tile_id) == MAN9 || \
   (tile_id) == PIN9 || \
   (tile_id) == SOU9)

#define IS_THREE(tile_id)   \
  ((tile_id) == MAN3 || \
   (tile_id) == PIN3 || \
   (tile_id) == SOU3)
#define IS_SEVEN(tile_id)   \
  ((tile_id) == MAN7 || \
   (tile_id) == PIN7 || \
   (tile_id) == SOU7)


typedef struct {
    uint8_t han;
    uint8_t fu;
    char yaku_name[MAX_YAKU_NAME_LEN];
} Score;

// chi, pon, kan(open and closed)
typedef struct {
    uint8_t tile_id[MAX_MELD_LEN]; // tile_id value should be specified.
    uint32_t len; // tile len. for chi or pon, 3 should be set, for kan, 4 should be set.
    uint8_t is_closed; // set true if closed-kan
} Meld;

typedef struct {
    Meld meld[MENTSU_LEN];
    uint32_t len;
} Melds;

typedef struct {
    uint8_t tile_id[MAX_HAND_LEN]; // tile_id value should be specified
    uint32_t len; // tile length
} Hands;

// return
//   0: success
//   -1: illegal params
// params
//   [out]
//     score: calculated Score
//   [in]
//     hands: all tiles include melds and win_tile
//     melds: list of meld
//     win_tile: win_tile
int32_t tile_get_score(
      Score *score,
      const Hands *hands,
      const Melds *melds,
      uint8_t win_tile,
      uint32_t is_ron,
      uint32_t player_wind,
      uint32_t round_wind);

