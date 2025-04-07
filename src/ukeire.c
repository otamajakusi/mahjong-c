/* MIT License
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
#include "ukeire.h"

#include <assert.h>
#include <stdio.h>

#include "mahjong.h"
#include "tile.h"

#define ENABLE_DEBUG (0)

// 1. 七対子、国士無双、通常手のシャンテン数を計算
// 2. 3つの中から最も少ないシャンテン数を選択
// 3. 選択したシャンテン数を減らす牌を列挙

static void reset_shanten_normal(ShantenCtx *ctx) {
  if (ctx->total_len % MJ_MIN_TILES_LEN_IN_ELEMENT == 2) {
    ctx->shanten_normal_min = -1;  // 和了
  } else {
    ctx->shanten_normal_min = 0;  // テンパイ
  }
  ctx->shanten_normal_max = ctx->total_len / MJ_MIN_TILES_LEN_IN_ELEMENT * 2 /*=2点*/;
  ctx->shanten_normal = ctx->shanten_normal_max;
}

static void incr_tile(ShantenCtx *ctx, MJTileId tile) {
  ctx->tiles.tiles[tile]++;
  ctx->total_len++;
}

static void decr_tile(ShantenCtx *ctx, MJTileId tile) {
  ctx->tiles.tiles[tile]--;
  ctx->total_len--;
}

int32_t init_ctx(ShantenCtx *ctx, const MJHands *hands) {
  memset(ctx, 0, sizeof(ShantenCtx));
  if (!gen_tiles_from_hands(&ctx->tiles, hands)) {
    return MJ_ERR_ILLEGAL_PARAM;
  }

  ctx->total_len = (int32_t)hands->len;
  reset_shanten_normal(ctx);
  return MJ_OK;
}

void gen_acceptable_kokushi(ShantenCtx *ctx, Tiles *acceptables) {
  // 現在のシャンテン数を取得
  calc_shanten_kokushi(ctx);
  int32_t current_shanten = ctx->shanten_kokushi;

  memset(acceptables, 0, sizeof(Tiles));
  const uint32_t yaochu[] = {MJ_M1, MJ_M9, MJ_P1, MJ_P9, MJ_S1, MJ_S9, MJ_WT, MJ_WN, MJ_WS, MJ_WP, MJ_DW, MJ_DG, MJ_DR};
  for (uint32_t i = 0; i < sizeof(yaochu) / sizeof(yaochu[0]); i++) {
    if (ctx->tiles.tiles[yaochu[i]] >= MJ_MAX_TILES_LEN_IN_ELEMENT) {
      continue;
    }
    incr_tile(ctx, yaochu[i]);
    calc_shanten_kokushi(ctx);
    decr_tile(ctx, yaochu[i]);
    if (ctx->shanten_kokushi < current_shanten) {
      acceptables->tiles[yaochu[i]] = 1;
    }
  }
}

void gen_acceptable_chiitoitsu(ShantenCtx *ctx, Tiles *acceptables) {
  // 現在のシャンテン数を取得
  calc_shanten_chiitoitsu(ctx);
  int32_t current_shanten = ctx->shanten_chiitoitsu;

  memset(acceptables, 0, sizeof(Tiles));
  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (ctx->tiles.tiles[i] == 1) {  // 2枚にしないとシャン点数は減らない
      incr_tile(ctx, i);
      calc_shanten_chiitoitsu(ctx);
      decr_tile(ctx, i);
      if (ctx->shanten_chiitoitsu < current_shanten) {
        acceptables->tiles[i] = 1;
      }
    }
  }
}

void gen_acceptable_normal(ShantenCtx *ctx, Tiles *acceptables) {
  // 現在のシャンテン数を取得
  calc_shanten_normal(ctx);
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
  fprintf(stderr, "-----------\nshanten: %d\n-----------\n", ctx->shanten_normal);
#endif
  int32_t current_shanten = ctx->shanten_normal;

  memset(acceptables, 0, sizeof(Tiles));

  // 有効牌候補を作成
  Tiles candidate;
  memset(&candidate, 0, sizeof(Tiles));
  // 数牌
  for (uint32_t i = MJ_M1; i <= MJ_S9; i++) {
    if (ctx->tiles.tiles[i] && ctx->tiles.tiles[i] < MJ_MAX_TILES_LEN_IN_ELEMENT) {
      uint32_t number = get_tile_number(i);
      switch (number) {
        case TILE_NUM_1:
          candidate.tiles[i] = 1;
          candidate.tiles[i + 1] = 1;
          candidate.tiles[i + 2] = 1;
          break;
        case TILE_NUM_9:
          candidate.tiles[i - 2] = 1;
          candidate.tiles[i - 1] = 1;
          candidate.tiles[i] = 1;
          break;
        case TILE_NUM_2:
          candidate.tiles[i - 1] = 1;
          candidate.tiles[i] = 1;
          candidate.tiles[i + 1] = 1;
          candidate.tiles[i + 2] = 1;
          break;
        case TILE_NUM_8:
          candidate.tiles[i - 2] = 1;
          candidate.tiles[i - 1] = 1;
          candidate.tiles[i] = 1;
          candidate.tiles[i + 1] = 1;
          break;
        default:
          candidate.tiles[i - 2] = 1;
          candidate.tiles[i - 1] = 1;
          candidate.tiles[i] = 1;
          candidate.tiles[i + 1] = 1;
          candidate.tiles[i + 2] = 1;
          break;
      }
    }
  }
  // 字牌
  for (uint32_t i = MJ_WT; i <= MJ_DR; i++) {
    if (ctx->tiles.tiles[i] && ctx->tiles.tiles[i] < MJ_MAX_TILES_LEN_IN_ELEMENT) {
      candidate.tiles[i] = 1;
    }
  }

  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (candidate.tiles[i] == 0 || ctx->tiles.tiles[i] >= MJ_MAX_TILES_LEN_IN_ELEMENT) {
      continue;
    }
    incr_tile(ctx, i);
    reset_shanten_normal(ctx);
    calc_shanten_normal(ctx);
    decr_tile(ctx, i);
#if defined(ENABLE_DEBUG) && (ENABLE_DEBUG >= 1)
    fprintf(stderr, "-----------\ni: %s, shanten: %d, current_shanten: %d\n-----------\n", tile_id_str(i),
            ctx->shanten_normal, current_shanten);
#endif
    if (ctx->shanten_normal < current_shanten) {
      acceptables->tiles[i] = 1;
    }
  }
}

int32_t mj_ukeire_kokushi(const MJHands *hands, MJTiles *acceptables) {
  ShantenCtx ctx;
  int32_t ret = init_ctx(&ctx, hands);
  if (ret != MJ_OK) {
    return ret;
  }
  gen_acceptable_kokushi(&ctx, acceptables);
  return MJ_OK;
}

int32_t mj_ukeire_chiitoitsu(const MJHands *hands, MJTiles *acceptables) {
  ShantenCtx ctx;
  int32_t ret = init_ctx(&ctx, hands);
  if (ret != MJ_OK) {
    return ret;
  }
  gen_acceptable_chiitoitsu(&ctx, acceptables);
  return MJ_OK;
}

int32_t mj_ukeire_normal(const MJHands *hands, MJTiles *acceptables) {
  int32_t ret;
  ShantenCtx ctx;
  ret = init_ctx(&ctx, hands);
  if (ret != MJ_OK) {
    return ret;
  }
  gen_acceptable_normal(&ctx, acceptables);
  return MJ_OK;
}
