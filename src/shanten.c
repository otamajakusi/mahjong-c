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

#include <assert.h>
#include <stdio.h>

#include "agari.h"
#include "mahjong.h"
#include "tile.h"

static uint32_t generate_elements_as_sequence(const Tiles *tiles, Elements *elems) {
  Tiles _tiles;
  memcpy(&_tiles, tiles, sizeof(Tiles));
  elems->len = 0;
  int i = MJ_M1;
  while (i <= MJ_S9) {
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
  return elems->len;
}

typedef struct {
  MJTileId tile_id[7];
  uint32_t len;
} _Pairs;

static void gen_pairs_candidates(const Tiles *tiles, _Pairs *pairs) {
  pairs->len = 0;
  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (tiles->tiles[i] >= MJ_PAIR_LEN) {
      pairs->tile_id[pairs->len] = i;
      pairs->len++;
    }
  }
}

static uint32_t gen_partial_sequence(const Tiles *tiles, Elements *elems) {
  Tiles _tiles;
  memcpy(&_tiles, tiles, sizeof(Tiles));
  elems->len = 0;
  int i = MJ_M1;
  while (i <= MJ_S9) {
    uint8_t tile_num = _tiles.tiles[i];
    if (tile_num == 0 || is_tile_id_honors(i) || get_tile_number(i) == 8 || _tiles.tiles[i + 1] == 0) {
      i++;
    } else {
      _tiles.tiles[i]--;
      _tiles.tiles[i + 1]--;
      assert(elems->len < MJ_ELEMENTS_LEN);
      elems->meld[elems->len].tile_id[0] = i;
      elems->meld[elems->len].tile_id[1] = i + 1;
      elems->meld[elems->len].len = 2;
      elems->meld[elems->len].concealed = true;
      elems->meld[elems->len].type = ELEM_TYPE_PARTIAL_SEQUENCE;  // ターツ
      elems->len++;
    }
  }
  return elems->len;
}

static void save_max(MJTileId tri0, MJTileId tri1, MJTileId tri2, uint32_t tri_len, const Elements *elements,
                     _Triplets *max_triplets, Elements *max_elements) {
  _Triplets triplets = {{tri0, tri1, tri2}, tri_len};
  fprintf(stderr, "max_triplets %d, max_elements %d, tri_len %d, elements %d\n", max_triplets->len, max_elements->len,
          tri_len, elements->len);
  if ((tri_len + elements->len) > (max_triplets->len + max_elements->len)) {
    memcpy(max_triplets, &triplets, sizeof(_Triplets));
    memcpy(max_elements, elements, sizeof(Elements));
  }
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
  const uint32_t num_elements = (hands->len - 1) / MJ_MIN_TILES_LEN_IN_ELEMENT;  // 面子(候補)の数
  if (num_elements == 0) {                                                       // 単騎待ち
    return 0;                                                                    // 0シャンテン
  }

  const uint32_t base = num_elements * 2;  // 面子は2点

  Tiles tiles;
  if (!gen_tiles_from_hands(&tiles, hands)) {
    return MJ_ERR_ILLEGAL_PARAM;
  }

  _Triplets triplets;
  gen_triplets_candidates(&triplets, &tiles);
  if (triplets.len == num_elements) {
    return 0;  // テンパイ
  }
  for (uint32_t i = 0; i < triplets.len; i++) {
    printf("triplets: %s(%d)\n", tile_id_str(triplets.tile_id[i]), triplets.tile_id[i]);
  }
  // シャン点数を計算する場合, 刻子候補と面子候補が同じならテンパイの為,
  // 刻子(候補)の数の最大は面子(候補)の数-1だけを考慮すればよい
  //
  // 面子(候補)の数1の場合
  // - 刻子(候補)の数0 -> すべて順子
  //
  // 面子(候補)の数2の場合
  // - 刻子(候補)の数0 -> すべて順子
  // - 刻子(候補)の数1 -> 刻子候補を抜いて残りがすべて順子
  //
  // 面子(候補)の数3の場合
  // - 刻子(候補)の数0 -> すべて順子
  // - 刻子(候補)の数1 -> すべて順子もしくは刻子候補を抜いて残りがすべて順子
  //                      e.g. 1刻子 112233345
  // - 刻子(候補)の数2 -> 刻子候補を1つ抜く必要はない. 残りで順子を構成できないから.
  //                      すべて順子もしくは刻子候補をすべて抜いて残りがすべて順子(e.g. 111222345)
  //
  // 面子(候補)の数4の場合
  // - 刻子(候補)の数0 -> すべて順子
  // - 刻子(候補)の数1 -> すべて順子もしくは刻子候補を抜いて残りがすべて順子(e.g. 123334455678x(すべて順子))
  // - 刻子(候補)の数2 -> すべて順子もしくは刻子候補1枚か2枚を抜いて残りがすべて順子
  //                      e.g. 4順子 123334455567x
  //                      e.g. 1刻子,3順子 123334455999x
  //                      e.g. 2刻子,2順子 111334455999x
  // - 刻子(候補)の数3 -> すべて順子もしくは刻子候補1枚か3枚を抜いて残りがすべて順子
  //                      e.g. 1刻子,3順子 333344455566x (=333 345 445566)
  //
  // -> 上記が満たされたらテンパイ

  _Triplets max_triplets = {0};
  Elements max_elements = {0};

  Elements elements;
  fprintf(stderr, "triplets: %d, num_elements: %d\n", triplets.len, num_elements);
  switch (num_elements) {
    case 1:
      if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
        return 0;  // テンパイ
      }
      save_max(0, 0, 0, 0, &elements, &max_triplets, &max_elements);
      break;
    case 2:
      switch (triplets.len) {
        case 0:
          if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
            return 0;  // テンパイ
          }
          save_max(0, 0, 0, 0, &elements, &max_triplets, &max_elements);
          break;
        case 1:
          tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 1 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[0], 0, 0, 0, &elements, &max_triplets, &max_elements);
          break;
      }
      break;
    case 3:
      switch (triplets.len) {
        case 0:
          if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
            return 0;  // テンパイ
          }
          save_max(0, 0, 0, 0, &elements, &max_triplets, &max_elements);
          break;
        case 1:
          if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
            return 0;  // テンパイ
          }
          save_max(0, 0, 0, 0, &elements, &max_triplets, &max_elements);

          tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 1 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[0], 0, 0, 1, &elements, &max_triplets, &max_elements);
          break;
        case 2:
          if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
            return 0;  // テンパイ
          }
          tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 2 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[0], triplets.tile_id[1], 0, 2, &elements, &max_triplets, &max_elements);
          break;
      }
      break;
    case 4:
      switch (triplets.len) {
        case 0:
          if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
            return 0;  // テンパイ
          }
          save_max(0, 0, 0, 0, &elements, &max_triplets, &max_elements);
          break;
        case 1:
          if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
            return 0;  // テンパイ
          }
          tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 1 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[0], 0, 0, 1, &elements, &max_triplets, &max_elements);
          break;
        case 2:
          if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
            return 0;  // テンパイ
          }
          save_max(0, 0, 0, 0, &elements, &max_triplets, &max_elements);

          // 刻子候補Aを1枚を抜く
          tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 1 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[0], 0, 0, 1, &elements, &max_triplets, &max_elements);

          // 刻子候補Bを1枚を抜く
          tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 1 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[1], 0, 0, 1, &elements, &max_triplets, &max_elements);

          // 刻子候補A,Bを抜く
          tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          printf("elements: %d, triplets: %d\n", elements.len, triplets.len);
          if (elements.len + 2 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[0], triplets.tile_id[1], 0, 2, &elements, &max_triplets, &max_elements);
          break;
        case 3:
          if (generate_elements_as_sequence(&tiles, &elements) == num_elements) {
            return 0;  // テンパイ
          }
          save_max(0, 0, 0, 0, &elements, &max_triplets, &max_elements);
          // 刻子候補Aを1枚を抜く
          tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 1 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[0], 0, 0, 1, &elements, &max_triplets, &max_elements);

          // 刻子候補Bを1枚を抜く
          tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 1 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[1], 0, 0, 1, &elements, &max_triplets, &max_elements);

          // 刻子候補Cを1枚を抜く
          tiles.tiles[triplets.tile_id[2]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[2]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 1 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[2], 0, 0, 1, &elements, &max_triplets, &max_elements);

          // 刻子候補A,B,Cを抜く
          tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          tiles.tiles[triplets.tile_id[2]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
          generate_elements_as_sequence(&tiles, &elements);
          tiles.tiles[triplets.tile_id[2]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;  // revert
          if (elements.len + 3 /*num removed*/ == num_elements) {
            return 0;  // テンパイ
          }
          save_max(triplets.tile_id[0], triplets.tile_id[1], triplets.tile_id[2], 3, &elements, &max_triplets,
                   &max_elements);
          break;
      }
      break;
  }

  fprintf(stderr, "max_triplets: %d, max_elements: %d\n", max_triplets.len, max_elements.len);

  // 刻子を抜く
  for (uint32_t i = 0; i < max_triplets.len; i++) {
    tiles.tiles[max_triplets.tile_id[i]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
  }

  // 順子を抜く
  for (uint32_t i = 0; i < max_elements.len; i++) {
    tiles.tiles[max_elements.meld[i].tile_id[0]]--;
    tiles.tiles[max_elements.meld[i].tile_id[1]]--;
    tiles.tiles[max_elements.meld[i].tile_id[2]]--;
  }

  // 対子を抜く
  _Pairs pairs;
  gen_pairs_candidates(&tiles, &pairs);
  for (uint32_t i = 0; i < pairs.len; i++) {
    tiles.tiles[pairs.tile_id[i]] -= MJ_PAIR_LEN;
  }

  // ターツを抜く
  Elements partial_sequences;
  gen_partial_sequence(&tiles, &partial_sequences);
  for (uint32_t i = 0; i < partial_sequences.len; i++) {
    tiles.tiles[partial_sequences.meld[i].tile_id[0]]--;
    tiles.tiles[partial_sequences.meld[i].tile_id[1]]--;
  }

  fprintf(stderr, "triplets: %d, sequences: %d, pairs: %d, partial_sequences: %d\n", max_triplets.len, max_elements.len,
          pairs.len, partial_sequences.len);
  int32_t shanten =
      (int32_t)base - (int32_t)(max_triplets.len * 2 + max_elements.len * 2 + pairs.len + partial_sequences.len);
  fprintf(stderr, "shanten: %d, base: %d\n", shanten, base);
  return shanten;
}
