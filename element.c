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
    for (uint32_t i = 0; i < elems->len; i ++) {
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

/* single element */
/* 2..8 */
bool is_element_chunchan(const Element *elem) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        if (is_tile_id_honors(i)) {
            return false;
        }
        uint32_t number = get_tile_number(i);
        if (number == 0 || number == 8) {
            return false;
        }
    }
    return true;
}

/* 1 or 9 */
bool is_element_routou(const Element *elem) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        if (is_tile_id_honors(i)) {
            return false;
        }
        uint32_t number = get_tile_number(i);
        if (number != 0 && number != 8) {
            return false;
        }
    }
    return true;
}

/* 1,9,字牌 */
bool is_element_yaochu(const Element *elem) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        if (is_tile_id_honors(i)) {
            continue;
        }
        uint32_t number = get_tile_number(i);
        if (number != 0 && number != 8) {
            return false;
        }
    }
    return true;
}

/* 123, 789, 111, 999 or 字牌 */
bool has_element_yaochu(const Element *elem) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        if (is_tile_id_honors(i)) {
            continue;
        }
        uint32_t number = get_tile_number(i);
        if (elem->type == ELEM_TYPE_SEQUENCE) {
            if (number != i && number != i + 6) { // 1,2,3 or 7,8,9
                return false;
            }
        } else {
            if (number != 0 && number != 8) { // 1 or 9
                return false;
            }
        }
    }
    return true;
}

bool is_element_sequence(const Element *elem) {
    return elem->type == ELEM_TYPE_SEQUENCE;
}

bool is_element_triplets(const Element *elem) {
    return elem->type == ELEM_TYPE_TRIPLETS;
}

bool is_element_fours(const Element *elem) {
    return elem->type == ELEM_TYPE_FOURS;
}

bool is_element_concealed(const Element *elem) {
    return elem->concealed;
}

/* all elements */
bool is_elements_chunchan(const Elements *elems);
bool is_elements_routou(const Elements *elems);
bool is_elements_yaochu(const Element *elems);
bool has_elements_routou(const Elements *elems);
bool is_elements_sequence(const Elements *elems);
bool is_elements_triplets(const Elements *elems);
bool is_elements_fours(const Elements *elems);
bool is_elements_concealed(const Elements *elems);

uint32_t count_elements_sequence(const Elements *elems);
uint32_t count_elements_triplets(const Elements *elems);
uint32_t count_elements_fours(const Elements *elems);
uint32_t count_elements_concealed_fours(const Elements *elems);
bool has_elements_melded(const Elements *elems); /* 暗槓はconcealed扱い */

bool is_ryanmen_machi(const Elements *elems, MJTileId win_tile);
bool is_tanki_machi(MJTileId pair_tile, MJTileId win_tile);
//bool is_same_element(const Element *e1, const Element *e2); // -> static function
/* count same elements count */
bool count_same_elements(const Elements *elems);

bool has_elements_tile_id(const Elements *elems, MJTileId tile_id);
