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
#include <string.h>

#include "element.h"
#include "mahjong.h"
#include "tile.h"

#if defined(__cplusplus)
extern "C" {
#endif  // defined(__cplusplus)

typedef bool AgariCallbackTiles(const Tiles *tiles, void *arg);
typedef bool AgariCallbackElements(const Elements *concealed, const Elements *melded, MJTileId pair, void *arg);

uint32_t find_agari(const Tiles *tiles, const Elements *elems, AgariCallbackTiles *cb_tiles,
                    AgariCallbackElements *cb_elements, void *cbarg);

/* save triplets tile id to tile_id[n]. e.g. if there is two triplets, 2 should be set for len */
typedef struct {
  MJTileId tile_id[MJ_ELEMENTS_LEN];
  uint32_t len;
} _Triplets;

void gen_triplets_candidates(_Triplets *triplets, const Tiles *tiles);

#if defined(__cplusplus)
}
#endif  // defined(__cplusplus)
