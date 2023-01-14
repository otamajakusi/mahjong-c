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
#include "element.h"

static int tile_cmp(const void *p1, const void *p2) {
    return *(const MJTileId*)p1 - *(const MJTileId*)p2;
}

static void sort_element(Element *elem) {
    qsort(&elem->tile_id[0], elem->len, sizeof(elem->tile_id[0]), tile_cmp);
}

static void sort_elements(Elements *elems) {
    for (int i = 0; i < elems->len; i ++) {
        sort_element(&elems->meld[i]);
    }
}

/*
 * before calling the following functions, elem should be sorted.
 */

static bool is_sequece(const Element *elem) {
    if (elem->len != 3) {
        return false;
    }
    uint32_t type = 0;
    for (uint32_t i = 0; i < elem->len; i ++) {
        type |= get_tile_type(elem->tile_id[i]);
    }
    /* make sure element contains only one type and the type is man, pin or sou */
    switch (type) {
        case TILE_TYPE_MAN:
        case TILE_TYPE_PIN:
        case TILE_TYPE_SOU:
            break;
        default:
            return false;
    }
    if (elem->tile_id[0] + 1 != elem->tile_id[1] ||
        elem->tile_id[1] + 1 != elem->tile_id[2]) {
        return false;
    }
    return true;
}

static bool is_triplets(const Element *elem) {
    if (elem->len != 3) {
        return false;
    }
    for (uint32_t i = 0; i < elem->len; i ++) {
        if (elem->tile_id[0] != elem->tile_id[i]) {
            return false;
        }
    }
    return true;
}

static bool is_fours(const Element *elem) {
    if (elem->len != 4) {
        return false;
    }
    for (uint32_t i = 0; i < elem->len; i ++) {
        if (elem->tile_id[0] != elem->tile_id[i]) {
            return false;
        }
    }
    return true;
}

bool gen_elements_from_melds(Elements *elems, const MJMelds *melds) {
    memcpy(elems, melds, sizeof(Elements));
    sort_elements(elems);
    for (uint32_t i = 0; i < elems->len; i ++) {
        Element *elem = &elems->meld[i];
        if (is_sequece(elem)) {
            elem->type = ELEM_TYPE_SEQUENCE;
        } else if (is_triplets(elem)) {
            elem->type = ELEM_TYPE_TRIPLETS;
        } else if (is_fours(elem)) {
            elem->type = ELEM_TYPE_FOURS;
        } else {
            return false;
        }
    }
    return true;
}
