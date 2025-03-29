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
// https://web.archive.org/web/20190402234201/http://cmj3.web.fc2.com/index.htm#syanten
// https://mahjong.ara.black/etc/shanten/index.htm

#include "shanten.h"

#include <assert.h>
#include <stdio.h>

#include "agari.h"
#include "mahjong.h"
#include "tile.h"

#define ENABLE_DEBUG (0)

#define DEBUG_DEPTH(depth)                                \
  do {                                                    \
    fprintf(stderr, "%d", depth);                         \
    for (int __depth = 0; __depth < (depth); __depth++) { \
      fprintf(stderr, " ");                               \
    }                                                     \
  } while (0)

static bool dig_partial(ShantenCtx *ctx, int depth) {
  ctx->stat_dig_partial++;
  for (int32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (ctx->elem_len + ctx->partial_len >= 4) {
      break;
    }
    if (ctx->tiles.tiles[i] >= MJ_PAIR_LEN) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i]);
#endif
      ctx->tiles.tiles[i] -= MJ_PAIR_LEN;
      ctx->partial_len++;
      bool limit = dig_partial(ctx, depth + 1);
      ctx->partial_len--;
      ctx->tiles.tiles[i] += MJ_PAIR_LEN;
      if (limit) {
        return true;
      }
    }
    uint32_t tile_number = get_tile_number(i);
    if (tile_number != TILE_NUM_INVALID) {  // 数牌
      if (tile_number <= TILE_NUM_8 && ctx->tiles.tiles[i] && ctx->tiles.tiles[i + 1]) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
        DEBUG_DEPTH(depth);
        fprintf(stderr, "%s(%d):%s(%d), %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i],
                tile_id_str(i + 1), ctx->tiles.tiles[i + 1]);
#endif
        ctx->tiles.tiles[i]--;
        ctx->tiles.tiles[i + 1]--;
        ctx->partial_len++;
        bool limit = dig_partial(ctx, depth + 1);
        ctx->partial_len--;
        ctx->tiles.tiles[i + 1]++;
        ctx->tiles.tiles[i]++;
        if (limit) {
          return true;
        }
      }
      if (tile_number <= TILE_NUM_7 && ctx->tiles.tiles[i] && ctx->tiles.tiles[i + 2]) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
        DEBUG_DEPTH(depth);
        fprintf(stderr, "%s(%d):%s(%d), %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i],
                tile_id_str(i + 2), ctx->tiles.tiles[i + 2]);
#endif
        ctx->tiles.tiles[i]--;
        ctx->tiles.tiles[i + 2]--;
        ctx->partial_len++;
        bool limit = dig_partial(ctx, depth + 1);
        ctx->partial_len--;
        ctx->tiles.tiles[i + 2]++;
        ctx->tiles.tiles[i]++;
        if (limit) {
          return true;
        }
      }
    }
  }
  int32_t shanten = ctx->elem_len * 2 + ctx->pair_len + ctx->partial_len;
  if (shanten > ctx->shanten_normal) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
    fprintf(stderr, "shanten %d (elem_len %d, partial_len %d)\n", shanten, ctx->elem_len, ctx->partial_len);
#endif
    ctx->shanten_normal = shanten;
    if (ctx->shanten_normal >= ctx->shanten_limit) {
      //fprintf(stderr, "shanten %d (elem_len %d, partial_len %d)\n", shanten, ctx->elem_len, ctx->partial_len);
      return true;
    }
  }
  return false;
}

static bool dig_element(ShantenCtx *ctx, int depth) {
  ctx->stat_dig_element++;
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 2)
  DEBUG_DEPTH(depth);
  fprintf(stderr, "%s(%d):depth %d\n", __func__, __LINE__, depth);
#endif
  for (int32_t i = MJ_M1; i <= MJ_DR; i++) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 2)
    DEBUG_DEPTH(depth);
    fprintf(stderr, "%s(%d):i %s\n", __func__, __LINE__, tile_id_str(i));
#endif
    if (ctx->tiles.tiles[i] >= MJ_MIN_TILES_LEN_IN_ELEMENT) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i]);
#endif
      ctx->tiles.tiles[i] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len++;
      bool limit = dig_element(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[i] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
    }
    uint32_t tile_number = get_tile_number(i);
    if (tile_number != TILE_NUM_INVALID &&  // 数牌
        tile_number <= TILE_NUM_7 && ctx->tiles.tiles[i] && ctx->tiles.tiles[i + 1] && ctx->tiles.tiles[i + 2]) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d), %s(%d), %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i],
              tile_id_str(i + 1), ctx->tiles.tiles[i + 1], tile_id_str(i + 2), ctx->tiles.tiles[i + 2]);
#endif
      ctx->tiles.tiles[i]--;
      ctx->tiles.tiles[i + 1]--;
      ctx->tiles.tiles[i + 2]--;
      ctx->elem_len++;
      bool limit = dig_element(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[i + 2]++;
      ctx->tiles.tiles[i + 1]++;
      ctx->tiles.tiles[i]++;
      if (limit) {
        return true;
      }
    }
  }
  return dig_partial(ctx, depth + 1);
}

static bool dig_sequence(ShantenCtx *ctx, int depth) {
  ctx->stat_dig_element++;
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 2)
  DEBUG_DEPTH(depth);
  fprintf(stderr, "%s(%d):depth %d\n", __func__, __LINE__, depth);
#endif
  for (int32_t i = MJ_M1; i <= MJ_DR; i++) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 2)
    DEBUG_DEPTH(depth);
    fprintf(stderr, "%s(%d):i %s\n", __func__, __LINE__, tile_id_str(i));
#endif
    uint32_t tile_number = get_tile_number(i);
    if (tile_number != TILE_NUM_INVALID &&  // 数牌
        tile_number <= TILE_NUM_7 && ctx->tiles.tiles[i] && ctx->tiles.tiles[i + 1] && ctx->tiles.tiles[i + 2]) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d), %s(%d), %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i],
              tile_id_str(i + 1), ctx->tiles.tiles[i + 1], tile_id_str(i + 2), ctx->tiles.tiles[i + 2]);
#endif
      ctx->tiles.tiles[i]--;
      ctx->tiles.tiles[i + 1]--;
      ctx->tiles.tiles[i + 2]--;
      ctx->elem_len++;
      bool limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[i + 2]++;
      ctx->tiles.tiles[i + 1]++;
      ctx->tiles.tiles[i]++;
      if (limit) {
        return true;
      }
    }
  }
  return dig_partial(ctx, depth + 1);
}

static bool dig_triplet(ShantenCtx *ctx, int depth) {
  _Triplets triplets;
  gen_triplets_candidates(&triplets, &ctx->tiles);
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
  DEBUG_DEPTH(depth);
  fprintf(stderr, "%s(%d):triplets.len %d\n", __func__, __LINE__, triplets.len);
#endif
  bool limit = false;
  limit = dig_sequence(ctx, depth + 1);
  if (limit) {
    return true;
  }
  switch (triplets.len) {
    case 1:
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[0]),
              ctx->tiles.tiles[triplets.tile_id[0]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len++;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
      break;
    case 2:
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[0]),
              ctx->tiles.tiles[triplets.tile_id[0]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len++;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[1]),
              ctx->tiles.tiles[triplets.tile_id[1]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len++;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d),%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[0]),
              ctx->tiles.tiles[triplets.tile_id[0]], tile_id_str(triplets.tile_id[1]),
              ctx->tiles.tiles[triplets.tile_id[1]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len += 2;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len -= 2;
      ctx->tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
      break;
    case 3:
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[0]),
              ctx->tiles.tiles[triplets.tile_id[0]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len++;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[1]),
              ctx->tiles.tiles[triplets.tile_id[1]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len++;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[2]),
              ctx->tiles.tiles[triplets.tile_id[2]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[2]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len++;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[triplets.tile_id[2]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d),%s(%d),%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[0]),
              ctx->tiles.tiles[triplets.tile_id[0]], tile_id_str(triplets.tile_id[1]),
              ctx->tiles.tiles[triplets.tile_id[1]], tile_id_str(triplets.tile_id[2]),
              ctx->tiles.tiles[triplets.tile_id[2]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[2]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len += 3;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len -= 3;
      ctx->tiles.tiles[triplets.tile_id[2]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
      break;
    case 4:
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      DEBUG_DEPTH(depth);
      fprintf(stderr, "%s(%d):%s(%d),%s(%d),%s(%d),%s(%d)\n", __func__, __LINE__, tile_id_str(triplets.tile_id[0]),
              ctx->tiles.tiles[triplets.tile_id[0]], tile_id_str(triplets.tile_id[1]),
              ctx->tiles.tiles[triplets.tile_id[1]], tile_id_str(triplets.tile_id[2]),
              ctx->tiles.tiles[triplets.tile_id[2]], tile_id_str(triplets.tile_id[3]),
              ctx->tiles.tiles[triplets.tile_id[3]]);
#endif
      ctx->tiles.tiles[triplets.tile_id[0]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[1]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[2]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[3]] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len += 4;
      limit = dig_sequence(ctx, depth + 1);
      ctx->elem_len -= 4;
      ctx->tiles.tiles[triplets.tile_id[3]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[2]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[1]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->tiles.tiles[triplets.tile_id[0]] += MJ_MIN_TILES_LEN_IN_ELEMENT;
      if (limit) {
        return true;
      }
      break;
  }
  return false;
}

static void dig(ShantenCtx *ctx) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 2)
  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    fprintf(stderr, "%s(%d): %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i]);
  }
#endif
  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (ctx->tiles.tiles[i] >= MJ_PAIR_LEN) {
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
      fprintf(stderr, "%s(%d):i %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i]);
#endif
      ctx->tiles.tiles[i] -= MJ_PAIR_LEN;
      ctx->pair_len++;
      ctx->stat_dig++;
      bool limit = dig_element(ctx, 1);
      ctx->pair_len--;
      ctx->tiles.tiles[i] += MJ_PAIR_LEN;
      if (limit) {
        return;
      }
    }
  }
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
  fprintf(stderr, "%s(%d): no pair\n", __func__, __LINE__);
#endif
  dig_element(ctx, 0);
}

void calc_shanten_kokushi(ShantenCtx *ctx) {
  int shanten = 0;
  int pair = 0;
  const uint32_t yaochu[] = {MJ_M1, MJ_M9, MJ_P1, MJ_P9, MJ_S1, MJ_S9, MJ_WT, MJ_WN, MJ_WS, MJ_WP, MJ_DW, MJ_DG, MJ_DR};
  for (uint32_t i = 0; i < sizeof(yaochu) / sizeof(yaochu[0]); i++) {
    if (ctx->tiles.tiles[yaochu[i]]) {
      shanten++;
      if (ctx->tiles.tiles[yaochu[i]] >= MJ_PAIR_LEN && pair == 0) {
        pair = 1;
      }
    }
  }
  ctx->shanten_kokushi = 13 - (shanten + pair);
}

void calc_shanten_chiitoitsu(ShantenCtx *ctx) {
  int shanten = 0;
  int kind = 0;
  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (ctx->tiles.tiles[i]) {
      kind++;
      if (ctx->tiles.tiles[i] >= MJ_PAIR_LEN) {
        shanten++;
      }
    }
  }
  // 11 22 33 44 55 55 7 => 1 -> 4枚組は7が入って5を捨てても1シャンテンにならない
  ctx->shanten_chiitoitsu = 6 - shanten;
  if (kind < 7) {
    ctx->shanten_chiitoitsu += 7 - kind;
  }
}

void calc_shanten_normal(ShantenCtx *ctx) {
  dig(ctx);

#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
  fprintf(stderr, "shanten %d\n", (ctx->total_len / MJ_MIN_TILES_LEN_IN_ELEMENT * 2) - ctx->shanten);
#endif
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 2)
  fprintf(stderr, "stat dig %d, stat dig element %d, stat dig partial %d\n", ctx->stat_dig, ctx->stat_dig_element,
          ctx->stat_dig_partial);
#endif
  ctx->shanten_normal = (int32_t)(ctx->total_len / MJ_MIN_TILES_LEN_IN_ELEMENT * 2 - ctx->shanten_normal);
}

/*
 */
int32_t mj_calc_shanten(const MJHands *hands, MJShanten *shanten) {
  (void)dig_triplet;
  (void)dig_sequence;
  ShantenCtx ctx;
  memset(&ctx, 0, sizeof(ctx));
  if (!gen_tiles_from_hands(&ctx.tiles, hands)) {
    return MJ_ERR_ILLEGAL_PARAM;
  }

  ctx.total_len = (int32_t)hands->len;
  if (ctx.total_len >= MJ_MIN_TILES_LEN_IN_ELEMENT * MJ_ELEMENTS_LEN + MJ_PAIR_LEN) {
    ctx.shanten_limit = 9;  // 和了
  } else {
    ctx.shanten_limit = ctx.total_len / MJ_MIN_TILES_LEN_IN_ELEMENT * 2;
  }

  if (ctx.total_len >= MJ_MIN_TILES_LEN_IN_ELEMENT * MJ_ELEMENTS_LEN + 1) {
    calc_shanten_kokushi(&ctx);
    shanten->kokushi = ctx.shanten_kokushi;
    calc_shanten_chiitoitsu(&ctx);
    shanten->chiitoitsu = ctx.shanten_chiitoitsu;
  }

  calc_shanten_normal(&ctx);
  shanten->normal = ctx.shanten_normal;

  return MJ_OK;
}
