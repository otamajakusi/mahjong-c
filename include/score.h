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

#include <stdbool.h>
#include <stdint.h>

#include "element.h"
#include "mahjong.h"
#include "tile.h"

#if defined(__cplusplus)
extern "C" {
#endif  // defined(__cplusplus)

typedef struct {
  MJTileId win_tile;
  bool ron;
  MJTileId player_wind;
  MJTileId round_wind;
} ScoreConfig;

bool calc_score(MJBaseScore *score, const Elements *concealed, const Elements *melded, MJTileId pair,
                const ScoreConfig *cfg);
/* for chiitoitsu and kokushi */
bool calc_score_with_tiles(MJBaseScore *score, const Tiles *tiles, const ScoreConfig *cfg);

/*
 * dealer false, tsumo false: score に振り込んだ人の支払いを設定する
 * dealer false, tsumo true: score に子の支払いを, scoreDealer に親の支払いを設定する
 * dealer true, tsumo false: score に振り込んだ人の支払いを設定する
 * dealer true, tsumo true: score に子の支払いを設定する
 */
void get_score(uint32_t fu, uint32_t han, bool tsumo, bool dealer, uint32_t *score, uint32_t *scoreDealer);

#if defined(__cplusplus)
}
#endif  // defined(__cplusplus)
