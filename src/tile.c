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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mahjong.h"
#include "tile.h"

static const char tile_id_to_str[][3] = {
    "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8", "m9",
    "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9",
    "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9",
    "wt", "wn", "ws", "wp",
    "dw", "dg", "dr",
};

bool is_tile_id_valid(MJTileId tile_id) {
    return (tile_id >= MJ_M1 && tile_id <= MJ_DR);
}

bool is_tile_id_honors(MJTileId tile_id) {
    return (tile_id >= MJ_WT && tile_id <= MJ_DR);
}

bool is_tile_id_wind(MJTileId tile_id) {
    return (tile_id >= MJ_WT && tile_id <= MJ_WP);
}

bool is_tile_id_dragon(MJTileId tile_id) {
    return (tile_id >= MJ_DW && tile_id <= MJ_DR);
}

bool is_tile_id_man(MJTileId tile_id) {
    return (tile_id >= MJ_M1 && tile_id <= MJ_M9);
}

bool is_tile_id_pin(MJTileId tile_id) {
    return (tile_id >= MJ_P1 && tile_id <= MJ_P9);
}

bool is_tile_id_sou(MJTileId tile_id) {
    return (tile_id >= MJ_S1 && tile_id <= MJ_S9);
}

bool is_tile_id_yaochu(MJTileId tile_id) {
    if (is_tile_id_honors(tile_id)) {
        return true;
    }
    uint32_t number = get_tile_number(tile_id);
    if (number == TILE_NUM_1 || number == TILE_NUM_9) {
        return true;
    }
    return false;
}

bool is_tile_id_routou(MJTileId tile_id) {
    if (is_tile_id_honors(tile_id)) {
        return false;
    }
    uint32_t number = get_tile_number(tile_id);
    if (number == TILE_NUM_1 || number == TILE_NUM_9) {
        return true;
    }
    return false;
}

uint32_t get_tile_type(MJTileId tile_id) {
    if (0) {
    } else if (is_tile_id_man(tile_id)) {
        return TILE_TYPE_MAN;
    } else if (is_tile_id_pin(tile_id)) {
        return TILE_TYPE_PIN;
    } else if (is_tile_id_sou(tile_id)) {
        return TILE_TYPE_SOU;
    } else if (is_tile_id_wind(tile_id)) {
        return TILE_TYPE_WIND;
    } else if (is_tile_id_dragon(tile_id)) {
        return TILE_TYPE_DRAGON;
    }
    return TILE_TYPE_INVALID;
}

uint32_t get_tile_number(MJTileId tile_id) {
    if (is_tile_id_man(tile_id) || 
        is_tile_id_pin(tile_id) || 
        is_tile_id_sou(tile_id)) {
        return tile_id % 9;
    }
    return -1u;
}

bool gen_tiles_from_hands(Tiles *tiles, const MJHands *hands) {
    memset(tiles, 0, sizeof(Tiles));
    for (uint32_t i = 0; i < hands->len; i ++) {
        uint32_t tile_id = hands->tile_id[i];
        if (tiles->tiles[tile_id] >= MJ_MAX_TILES_LEN_IN_ELEMENT) {
            return false;
        }
        tiles->tiles[tile_id] ++;
    }
    return true;
}


const char* tile_id_str(MJTileId tile_id) {
    return tile_id_to_str[tile_id];
}
