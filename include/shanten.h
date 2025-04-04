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

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "tile.h"

#if defined(__cplusplus)
extern "C" {
#endif  // defined(__cplusplus)

typedef struct {
  int32_t shanten_normal;
  int32_t shanten_chiitoitsu;
  int32_t shanten_kokushi;
  int32_t shanten_normal_min;
  int32_t shanten_normal_max;
  Tiles tiles;
  int32_t total_len;
  int32_t elem_len;
  int32_t pair_len;
  int32_t partial_len;
  int32_t stat_dig;
  int32_t stat_dig_element;
  int32_t stat_dig_partial;
} ShantenCtx;

void calc_shanten_kokushi(ShantenCtx *ctx);
void calc_shanten_chiitoitsu(ShantenCtx *ctx);
void calc_shanten_normal(ShantenCtx *ctx);

#if defined(__cplusplus)
}
#endif  // defined(__cplusplus)
