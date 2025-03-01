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

#include <assert.h>
#include <stdio.h>

#include "agari.h"
#include "mahjong.h"
#include "tile.h"

#define ENABLE_DEBUG 0

typedef struct {
  int32_t shanten;
  Tiles tiles;
  int32_t elem_len;
  int32_t pair_len;
  int32_t partial_len;
} ShantenCtx;

static void dig_partial(ShantenCtx *ctx, int depth) {
  for (int32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (ctx->elem_len + ctx->partial_len >= 4) {
      break;
    }
    if (ctx->tiles.tiles[i] >= MJ_PAIR_LEN) {
#if ENABLE_DEBUG
      for (int s = 0; s < depth; s++) {
        fprintf(stderr, " ");
      }
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i]);
#endif  //if ENABLE_DEBUG
      ctx->tiles.tiles[i] -= MJ_PAIR_LEN;
      ctx->partial_len++;
      dig_partial(ctx, depth + 1);
      ctx->partial_len--;
      ctx->tiles.tiles[i] += MJ_PAIR_LEN;
    }
    uint32_t tile_number = get_tile_number(i);
    if (tile_number != TILE_NUM_INVALID) {  // 数牌
      if (tile_number <= TILE_NUM_8 && ctx->tiles.tiles[i] && ctx->tiles.tiles[i + 1]) {
#if ENABLE_DEBUG
        for (int s = 0; s < depth; s++) {
          fprintf(stderr, " ");
        }
        fprintf(stderr, "%s(%d):%s(%d), %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i],
                tile_id_str(i + 1), ctx->tiles.tiles[i + 1]);
#endif  //if ENABLE_DEBUG
        ctx->tiles.tiles[i]--;
        ctx->tiles.tiles[i + 1]--;
        ctx->partial_len++;
        dig_partial(ctx, depth + 1);
        ctx->partial_len--;
        ctx->tiles.tiles[i + 1]++;
        ctx->tiles.tiles[i]++;
      }
      if (tile_number <= TILE_NUM_7 && ctx->tiles.tiles[i] && ctx->tiles.tiles[i + 2]) {
#if ENABLE_DEBUG
        for (int s = 0; s < depth; s++) {
          fprintf(stderr, " ");
        }
        fprintf(stderr, "%s(%d):%s(%d), %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i],
                tile_id_str(i + 2), ctx->tiles.tiles[i + 2]);
#endif  //if ENABLE_DEBUG
        ctx->tiles.tiles[i]--;
        ctx->tiles.tiles[i + 2]--;
        ctx->partial_len++;
        dig_partial(ctx, depth + 1);
        ctx->partial_len--;
        ctx->tiles.tiles[i + 2]++;
        ctx->tiles.tiles[i]++;
      }
    }
  }
  int32_t shanten = ctx->elem_len * 2 + ctx->pair_len + ctx->partial_len;
  if (shanten > ctx->shanten) {
#if ENABLE_DEBUG
    fprintf(stderr, "shanten %d (elem_len %d, partial_len %d)\n", shanten, ctx->elem_len, ctx->partial_len);
#endif  //if ENABLE_DEBUG
    ctx->shanten = shanten;
  }
}

static void dig_element(ShantenCtx *ctx, int depth) {
#if 0   //ENABLE_DEBUG
  for (int s = 0; s < depth; s++) {
    fprintf(stderr, " ");
  }
  fprintf(stderr, "%s(%d):depth %d\n", __func__, __LINE__, depth);
#endif  //if ENABLE_DEBUG
  for (int32_t i = MJ_M1; i <= MJ_DR; i++) {
#if 0   // ENABLE_DEBUG
    for (int s = 0; s < depth; s++) {
      fprintf(stderr, " ");
    }
    fprintf(stderr, "%s(%d):i %s\n", __func__, __LINE__, tile_id_str(i));
#endif  //if ENABLE_DEBUG
    if (ctx->tiles.tiles[i] >= MJ_MIN_TILES_LEN_IN_ELEMENT) {
#if ENABLE_DEBUG
      for (int s = 0; s < depth; s++) {
        fprintf(stderr, " ");
      }
      fprintf(stderr, "%s(%d):%s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i]);
#endif  //if ENABLE_DEBUG
      ctx->tiles.tiles[i] -= MJ_MIN_TILES_LEN_IN_ELEMENT;
      ctx->elem_len++;
      dig_element(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[i] += MJ_MIN_TILES_LEN_IN_ELEMENT;
    }
    uint32_t tile_number = get_tile_number(i);
    if (tile_number != TILE_NUM_INVALID &&  // 数牌
        tile_number <= TILE_NUM_7 && ctx->tiles.tiles[i] && ctx->tiles.tiles[i + 1] && ctx->tiles.tiles[i + 2]) {
#if ENABLE_DEBUG
      for (int s = 0; s < depth; s++) {
        fprintf(stderr, " ");
      }
      fprintf(stderr, "%s(%d):%s(%d), %s(%d), %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i],
              tile_id_str(i + 1), ctx->tiles.tiles[i + 1], tile_id_str(i + 2), ctx->tiles.tiles[i + 2]);
#endif  //if ENABLE_DEBUG
      ctx->tiles.tiles[i]--;
      ctx->tiles.tiles[i + 1]--;
      ctx->tiles.tiles[i + 2]--;
      ctx->elem_len++;
      dig_element(ctx, depth + 1);
      ctx->elem_len--;
      ctx->tiles.tiles[i + 2]++;
      ctx->tiles.tiles[i + 1]++;
      ctx->tiles.tiles[i]++;
    }
  }
  dig_partial(ctx, depth + 1);
}

static void dig(ShantenCtx *ctx) {
#if 0   // ENABLE_DEBUG
  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    fprintf(stderr, "%s(%d): %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i]);
  }
#endif  //if ENABLE_DEBUG
  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (ctx->tiles.tiles[i] >= MJ_PAIR_LEN) {
#if ENABLE_DEBUG
      fprintf(stderr, "%s(%d):i %s(%d)\n", __func__, __LINE__, tile_id_str(i), ctx->tiles.tiles[i]);
#endif  //if ENABLE_DEBUG
      ctx->tiles.tiles[i] -= MJ_PAIR_LEN;
      ctx->pair_len++;
      dig_element(ctx, 1);
      ctx->pair_len--;
      ctx->tiles.tiles[i] += MJ_PAIR_LEN;
    }
  }
#if ENABLE_DEBUG
  fprintf(stderr, "%s(%d): no pair\n", __func__, __LINE__);
#endif  //if ENABLE_DEBUG
  dig_element(ctx, 0);
}

int32_t mj_calc_shanten(const MJHands *hands) {
  /*
  if (hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 0 + 1 && hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 1 + 1 &&
      hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 2 + 1 && hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 3 + 1 &&
      hands->len != MJ_MIN_TILES_LEN_IN_ELEMENT * 4 + 1) {
    return MJ_ERR_ILLEGAL_PARAM;
  }
  */

  ShantenCtx ctx;
  if (!gen_tiles_from_hands(&ctx.tiles, hands)) {
    return MJ_ERR_ILLEGAL_PARAM;
  }

  ctx.shanten = 0;
  ctx.elem_len = 0;
  ctx.pair_len = 0;
  ctx.partial_len = 0;

  dig(&ctx);

#if ENABLE_DEBUG
  fprintf(stderr, "shanten %d\n", 8 - ctx.shanten);
#endif  //if ENABLE_DEBUG
  return (int32_t)(8 - ctx.shanten);
}
