/*
 *  MIT License
 *
 *  Copyright (c) 2025 otamajakusi
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

#include "agari.h"
#include "mahjong.h"
#include "tile.h"

static void generate_elements_as_sequence(const Tiles *tiles, Elements *elems, uint32_t begin, uint32_t end) {
  Tiles _tiles;
  memcpy(&_tiles, tiles, sizeof(Tiles));
  int i = begin;
  while (i <= end) {
    uint8_t tile_num = _tiles.tiles[i];
    if (tile_num == 0 || is_tile_id_honors(i) || get_tile_number(i) == 7 || get_tile_number(i) == 8 ||
        _tiles.tiles[i + 1] == 0 || _tiles.tiles[i + 2] == 0) {
      i++;
    } else {
      _tiles.tiles[i]--;
      _tiles.tiles[i + 1]--;
      _tiles.tiles[i + 2]--;
      assert(elems->len < MJ_ELEMENTS_LEN);
      elems->meld[elems->len].tile_id[0] = i;
      elems->meld[elems->len].tile_id[1] = i + 1;
      elems->meld[elems->len].tile_id[2] = i + 2;
      elems->meld[elems->len].len = 3;
      elems->meld[elems->len].concealed = true;
      elems->meld[elems->len].type = ELEM_TYPE_SEQUENCE;
      elems->len++;
    }
  }
  return;
}

/* TilesからElementsを作成する */
static int32_t generate_elements(const Tiles *tiles, Elements *elements) {
  (void)elements;
  _Triplets triplets;
  gen_triplets_candidates(&triplets, tiles);

  printf("triplets: ");
  for (uint32_t i = 0; i < triplets.len; i++) {
    printf("%s ", tile_id_str(triplets.tile_id[i]));
  }
  printf("\n");

  Elements _elements;
  memset(&_elements, 0, sizeof(Elements));
  // agari判定と異なり例えば萬子の数が3の倍数でなくても面子を作成する
  for (uint32_t i = MJ_M1; i <= MJ_M9; i++) {  // 萬子
                                               // すべてsequenceか
  }
  return 0;
}

/*
 * handsからシャン点数を計算する
 * handsは手配のみとし、副露牌は含まない
 * 副露牌を含まないのでhandsの数は
 * - 1(=3*0+1)
 * - 4(=3*1+1)
 * - 7(=3*2+1)
 * - 10(=3*3+1)
 * - 13(=3*4+1)
 * のいずれか
 * https://note.com/mleague_results/n/nfe3e0e8774db
 */
int32_t mj_calc_shanten(const MJHands *hands) {
  if (hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 0 + 1 && hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 1 + 1 &&
      hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 2 + 1 && hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 3 + 1 &&
      hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 4 + 1) {
    return MJ_ERR_ILLEGAL_PARAM;
  }
  const uint32_t num_elements = 4 - (hands->len - 1) / MJ_MIN_TILES_LEN_IN_ELEMENT;
  const uint32_t base = 8 - num_elements * 2;  // 面子は2点
  if (base == 0) {                             // 単騎待ちはテンパイ
    return 0;                                  // 0シャンテン
  }

  Tiles tiles;
  if (!gen_tiles_from_hands(&tiles, hands)) {
    return MJ_ERR_ILLEGAL_PARAM;
  }

  _Triplets triplets;
  gen_triplets_candidates(&triplets, tiles);
  if (triplets.len == num_elements) {
    return 0;  // テンパイ
  }
  // シャン点数を計算する場合, 刻子(候補)の数の最大は面子(候補)の数-1.
  // 面子(候補)の数1
  // - 刻子(候補)の数0 -> すべて順子
  //
  // 面子(候補)の数2
  // - 刻子(候補)の数0 -> すべて順子
  // - 刻子(候補)の数1 -> 刻子候補を抜いて残りがすべて順子
  //
  // 面子(候補)の数3
  // - 刻子(候補)の数0 -> すべて順子
  // - 刻子(候補)の数1 -> すべて順子もしくは刻子候補を抜いて残りがすべて順子(e.g. 112233345)
  // - 刻子(候補)の数2 -> 刻子候補を1つ抜く必要はない. 残りで順子を構成できないから.
  //                      すべて順子もしくは刻子候補をすべて抜いて残りがすべて順子(e.g. 111222345)
  //
  // 面子(候補)の数4
  // - 刻子(候補)の数0 -> (刻子候補を抜いて)残りはすべて順子
  // - 刻子(候補)の数1 -> すべて順子もしくは刻子候補を抜いて残りがすべて順子
  // - 刻子(候補)の数2 -> すべて順子もしくは刻子候補1枚か2枚を抜いて残りがすべて順子
  // - 刻子(候補)の数3 -> すべて順子もしくは刻子候補1枚か3枚を抜いて残りがすべて順子

  // 面子・対子・ターツの順に取り出す
  for (int32_t i = 0; i < (int32_t)(sizeof(tiles.tiles) / sizeof(tiles.tiles[0])); i++) {
    if (tiles.tiles[i] == 0) {
      continue;
    }
    printf("%s: %d\n", tile_id_str(i), tiles.tiles[i]);
  }

  // num_meldedが0の場合は、4面子1雀頭、七対子、国士無双も判定し最も小さいシャン点数を返す
  // 123334455aceX を判定することを考える (b, d, Xが入るとテンパイの1シャンテン)
  // 11222333 この場合
  //   123, 123, 23にすると2面子, 1ターツになる
  //   11, 222, 333にすると2面子, 1対子になる -> 対子を優先するためこちらを選ぶ必要がある
  // 11122333
  // 12222333
  //
  // 面子(順子・刻子)の最大数を取り出す
  // 萬子, 筒子, 索子に分割して考える
  Elements elements;
  generate_elements(&tiles, &elements);
  return 0;
}
