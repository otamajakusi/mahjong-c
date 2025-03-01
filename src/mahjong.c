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

#include "mahjong.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agari.h"
#include "element.h"
#include "hand.h"
#include "meld.h"
#include "score.h"
#include "tile.h"

static bool remove_melds_from_tiles(Tiles *tiles, const MJMelds *melds) {
  for (uint32_t i = 0; i < melds->len; i++) {
    const MJMeld *meld = &melds->meld[i];
    for (uint32_t j = 0; j < meld->len; j++) {
      MJTileId tile_id = meld->tile_id[j];
      if (tiles->tiles[tile_id] == 0) {
        fprintf(stderr, "tile in meld %d doesn't exists in hands\n", meld->tile_id[j]);
        return false;
      }
      tiles->tiles[tile_id]--;
    }
  }
  return true;
}

/*
 * [複数アガリの点数の比較]
 * 同じアガリ配で複数通りのアガリがある場合に翻と符を必ず比較しないといけないか？
 * たとえば複数のアガリ A. 60符1翻, B. 30符2翻 があり得るか？
 * 場合:
 * 40符1翻と20符2翻(子1300)
 * 50符1翻と25符2翻(子1600)
 * 60符1翻と30符2翻(子2000)
 * 80符1翻と40符2翻(子2600)
 * 40符2翻と20符3翻(子2600)
 * 50符2翻と25符3翻(子3200)
 * 60符2翻と30符3翻(子3900)
 * 25符は七対子の場合で七対子が面子役になるためには二盃口となり50符1翻にならない.
 * 同じアガリ配で大きく異なる符となるためには順子構成が刻子構成にもなる場合で
 * 例えば 123,123,123,678,99 と 111,222,333,678,99 の場合
 * 20符2翻(平和,**自摸**)なので後者は常に三暗刻が付き1翻で止まらない.
 * 30符2翻と60符1翻は30の符の差があり, これは順子<->刻子の構成だけでは作れない.
 * 123,123,123 を 111,222,333 と捉えた場合16符の差しかない.
 * つまり翻を比較して同じなら符を比較して大小を比較できる.
 */

typedef struct {
  MJBaseScore score;
  ScoreConfig score_config;
} _Score;

/* for 国士無双, 七対子 */
static bool score_tiles(const Tiles *tiles, void *arg) {
  _Score *_score = (_Score *)arg;
  MJBaseScore score;
  bool agari = calc_score_with_tiles(&score, tiles, &_score->score_config);
  // save greater score
  if (score.han) {
    fprintf(stderr, "%s\n", score.yaku_name);
  }
  if ((score.han > _score->score.han) || (score.han == _score->score.han && score.fu > _score->score.fu)) {
    fprintf(stderr, "changed %d:%d:%s --> %d:%d:%s\n", _score->score.han, _score->score.fu, _score->score.yaku_name,
            score.han, score.fu, score.yaku_name);
    memcpy(&_score->score, &score, sizeof(MJBaseScore));
  }
  return agari;
}

static bool score_elements(const Elements *concealed, const Elements *melded, MJTileId pair, void *arg) {
  _Score *_score = (_Score *)arg;
  MJBaseScore score;
  bool agari = calc_score(&score, concealed, melded, pair, &_score->score_config);
  if (score.han) {
    fprintf(stderr, "%s\n", score.yaku_name);
  }
  // save greater score
  if ((score.han > _score->score.han) || (score.han == _score->score.han && score.fu > _score->score.fu)) {
    fprintf(stderr, "changed %d:%d:%s --> %d:%d:%s\n", _score->score.han, _score->score.fu, _score->score.yaku_name,
            score.han, score.fu, score.yaku_name);
    memcpy(&_score->score, &score, sizeof(MJBaseScore));
  }
  return agari;
}

int32_t mj_get_score(MJBaseScore *score, const MJHands *hands, const MJMelds *melds, MJTileId win_tile, bool ron,
                     MJTileId player_wind, MJTileId round_wind) {
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
  if (win_tile < MJ_M1 || win_tile > MJ_DR) {
    return MJ_ERR_ILLEGAL_PARAM;
  }

  Tiles tiles;
  if (!gen_tiles_from_hands(&tiles, hands)) {
    return MJ_ERR_ILLEGAL_PARAM;
  }
  // make tiles = hands - melds
  if (!remove_melds_from_tiles(&tiles, melds)) {
    return MJ_ERR_ILLEGAL_PARAM;
  }
  if (tiles.tiles[win_tile] == 0) {  // check if win_tile is in tiles
    return MJ_ERR_ILLEGAL_PARAM;
  }

  Elements melded_elems;
  if (!gen_elements_from_melds(&melded_elems, melds)) {
    return MJ_ERR_ILLEGAL_PARAM;
  }

  _Score _score = {
      {0, 0, ""},
      {win_tile, ron, player_wind, round_wind},
  };

  uint32_t agari = find_agari(&tiles, &melded_elems, score_tiles, score_elements, &_score);
  if (agari == 0) {
    return MJ_ERR_AGARI_NOT_FOUND;
  }
  memcpy(score, &_score.score, sizeof(MJBaseScore));
  return MJ_OK;
}
