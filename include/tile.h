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

#include "mahjong.h"

#if defined(__cplusplus)
extern "C" {
#endif  // defined(__cplusplus)

#define TILE_TYPE_MAN (1u << 0)
#define TILE_TYPE_PIN (1u << 1)
#define TILE_TYPE_SOU (1u << 2)
#define TILE_TYPE_WIND (1u << 3)
#define TILE_TYPE_DRAGON (1u << 4)
#define TILE_TYPE_INVALID (1u << 7)

#define TILE_NUM_1 (1u - 1)
#define TILE_NUM_2 (2u - 1)
#define TILE_NUM_3 (3u - 1)
#define TILE_NUM_4 (4u - 1)
#define TILE_NUM_5 (5u - 1)
#define TILE_NUM_6 (6u - 1)
#define TILE_NUM_7 (7u - 1)
#define TILE_NUM_8 (8u - 1)
#define TILE_NUM_9 (9u - 1)
#define TILE_NUM_INVALID (-1u)

typedef struct {
  MJTileId tiles[MJ_DR + 1];
} Tiles;

bool is_tile_id_valid(MJTileId tile_id);

bool is_tile_id_honors(MJTileId tile_id);
bool is_tile_id_wind(MJTileId tile_id);
bool is_tile_id_dragon(MJTileId tile_id);
bool is_tile_id_man(MJTileId tile_id);
bool is_tile_id_pin(MJTileId tile_id);
bool is_tile_id_sou(MJTileId tile_id);
/* 1,9字牌 */
bool is_tile_id_yaochu(MJTileId tile_id);
/* 1,9牌 */
bool is_tile_id_routou(MJTileId tile_id);

uint32_t get_tile_type(MJTileId tile_id);
uint32_t get_tile_number(MJTileId tile_id);  // for man, pin and sou

bool gen_tiles_from_hands(Tiles *tiles, const MJHands *hands);

const char *tile_id_str(MJTileId tile_id);
#if defined(__cplusplus)
}
#endif  // defined(__cplusplus)
