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

#define ELEMS_FOR_EACH(elems, callback) \
    for (uint32_t i = 0; i < (elems)->len; i ++) { \
        const Element *elem = &elems->meld[i]; \
        if (!callback(elem)) { \
            return false; \
        } \
    } \
    return true

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
        MJTileId tile_id = elem->tile_id[i];
        if (is_tile_id_yaochu(tile_id)) {
            return false;
        }
    }
    return true;
}

/* 1 or 9 */
bool is_element_routou(const Element *elem) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        MJTileId tile_id = elem->tile_id[i];
        if (!is_tile_id_routou(tile_id)) {
            return false;
        }
    }
    return true;
}

bool has_element_routou(const Element *elem) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        MJTileId tile_id = elem->tile_id[i];
        if (is_tile_id_honors(tile_id)) {
            return false;
        }
        uint32_t number = get_tile_number(tile_id);
        if (elem->type == ELEM_TYPE_SEQUENCE) {
            if (number != (TILE_NUM_1 + i) && number != (TILE_NUM_7 + i)) { // 1,2,3 or 7,8,9
                return false;
            }
        } else {
            if (number != TILE_NUM_1 && number != TILE_NUM_9) { // 1 or 9
                return false;
            }
        }
    }
    return true;
}

/* 1,9,字牌 */
bool is_element_yaochu(const Element *elem) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        MJTileId tile_id = elem->tile_id[i];
        if (!is_tile_id_yaochu(tile_id)) {
            return false;
        }
    }
    return true;
}

/* 123, 789, 111, 999 or 字牌 */
bool has_element_yaochu(const Element *elem) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        MJTileId tile_id = elem->tile_id[i];
        if (is_tile_id_honors(tile_id)) {
            continue;
        }
        uint32_t number = get_tile_number(tile_id);
        if (elem->type == ELEM_TYPE_SEQUENCE) {
            if (number != (TILE_NUM_1 + i) && number != (TILE_NUM_7 + i)) { // 1,2,3 or 7,8,9
                return false;
            }
        } else {
            if (number != TILE_NUM_1 && number != TILE_NUM_9) { // 1 or 9
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
bool is_elements_chunchan(const Elements *elems) {
    ELEMS_FOR_EACH(elems, is_element_chunchan);
}

bool is_elements_routou(const Elements *elems) {
    ELEMS_FOR_EACH(elems, is_element_routou);
}

bool has_elements_routou(const Elements *elems) {
    ELEMS_FOR_EACH(elems, has_element_routou);
}

bool is_elements_yaochu(const Elements *elems) {
    ELEMS_FOR_EACH(elems, is_element_yaochu);
}

bool has_elements_yaochu(const Elements *elems) {
    ELEMS_FOR_EACH(elems, has_element_yaochu);
}

bool is_elements_sequence(const Elements *elems) {
    ELEMS_FOR_EACH(elems, is_element_sequence);
}

bool is_elements_triplets(const Elements *elems) {
    ELEMS_FOR_EACH(elems, is_element_triplets);
}

bool is_elements_fours(const Elements *elems) {
    ELEMS_FOR_EACH(elems, is_element_fours);
}

bool is_elements_concealed(const Elements *elems) {
    ELEMS_FOR_EACH(elems, is_element_concealed);
}

uint32_t count_elements_sequence(const Elements *elems) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < elems->len; i ++) {
       if (is_element_sequence(&elems->meld[i])) {
           count ++;
       }
    }
    return count;
}

uint32_t count_elements_triplets(const Elements *elems) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < elems->len; i ++) {
       if (is_element_triplets(&elems->meld[i])) {
           count ++;
       }
    }
    return count;
}

uint32_t count_elements_fours(const Elements *elems) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < elems->len; i ++) {
       if (is_element_fours(&elems->meld[i])) {
           count ++;
       }
    }
    return count;
}

uint32_t count_elements_concealed_fours(const Elements *elems) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < elems->len; i ++) {
       if (is_element_fours(&elems->meld[i]) && is_element_concealed(&elems->meld[i])) {
           count ++;
       }
    }
    return count;
}

bool has_elements_melded(const Elements *elems) { /* 暗槓はconcealed扱い */
    for (uint32_t i = 0; i < elems->len; i ++) {
       if (!is_element_concealed(&elems->meld[i])) {
           return true;
       }
    }
    return false;
}

bool is_ryanmen_machi(const Elements *elems, MJTileId win_tile) {
    for (uint32_t i = 0; i < elems->len; i ++) {
        const Element *elem = &elems->meld[i];
        if (!is_element_sequence(elem)) {
            continue;
        }
        MJTileId tile_id;
        uint32_t number;

        tile_id = elem->tile_id[0];
        number = get_tile_number(tile_id);
        assert(number <= TILE_NUM_7);
        if (tile_id == win_tile && number != TILE_NUM_7) { // n,n+1,n+2の順子でアガリ牌nが7でなければ両面待ち
            return true;
        }
        tile_id = elem->tile_id[2];
        number = get_tile_number(tile_id);
        assert(number >= TILE_NUM_3);
        if (tile_id == win_tile && number != TILE_NUM_3) { // n,n+1,n+2の順子でアガリ牌n+2が3でなければ両面待ち
            return true;
        }
    }
    return false;
}

/* 三暗刻, 四暗刻のアガリの刻子を調べる */
bool is_shanpon_machi(const Elements *elems, MJTileId win_tile) {
    for (uint32_t i = 0; i < elems->len; i ++) {
        const Element *elem = &elems->meld[i];
        if (!is_element_triplets(elem)) {
            continue;
        }
        MJTileId tile_id = elem->tile_id[0];
        if (tile_id == win_tile) {
            return true;
        }
    }
    return false;
}

bool is_tanki_machi(MJTileId pair_tile, MJTileId win_tile) {
    return pair_tile == win_tile;
}

static bool is_element_same_sequence(const Element *e1, const Element *e2) {
    if (e1->len != e2->len) {
        return false;
    }
    if (e1->type != e2->type || e1->type != ELEM_TYPE_SEQUENCE) {
        return false;
    }
    if (e1->concealed != e2->concealed) {
        return false;
    }
    for (uint32_t i = 0; i < e1->len; i ++) {
        if (e1->tile_id[i] != e2->tile_id[i]) {
            return false;
        }
    }
    return true;
}

static uint32_t count_elements_same_sequence_from(uint32_t start, const Elements *elems) {
    uint32_t count = 0;
    assert(start < elems->len);
    const Element *elem = &elems->meld[start];
    for (uint32_t i = start + 1; i < elems->len; i ++) {
        if (is_element_same_sequence(elem, &elems->meld[i])) {
            count ++;
        }
    }
    return count;
}
/*
 * counts same sequence element in elements
 * 123, 123, 123, 123 => 6
 * 123, 123, 123, xxx => 3
 * 123, 123, yyy, xxx => 1
 * 123, zzz, yyy, xxx => 0
 * 123, 456, 123, 456 => 2
 */

uint32_t count_elements_same_sequence(const Elements *elems) {
    if (elems->len <= 1) {
        return 0;
    }
    uint32_t count = 0;
    for (uint32_t i = 0; i < elems->len - 1; i ++) {
        count += count_elements_same_sequence_from(i, elems);
    }
    return count;
}

static int has_element_tile_id(const Element *elem, MJTileId tile_id) {
    for (uint32_t i = 0; i < elem->len; i ++) {
        if (elem->tile_id[i] == tile_id) {
            return true;
        }
    }
    return false;
}

bool has_elements_tile_id(const Elements *elems, MJTileId tile_id) {
    for (uint32_t i = 0; i < elems->len; i ++) {
        if (has_element_tile_id(&elems->meld[i], tile_id)) {
            return true;
        }
    }
    return false;
}

/* merge s1 and s3 melds to dst */
void merge_elements(Elements *dst, const Elements *e1, const Elements *e2) {
    dst->len = 0;
    Element *e = &dst->meld[0];
    for (uint32_t i = 0; i < e1->len; i ++) {
        memcpy(e, &e1->meld[i], sizeof(Element));
        e ++;
        assert(dst->len < MJ_ELEMENTS_LEN);
        dst->len ++;
    }
    for (uint32_t i = 0; i < e2->len; i ++) {
        memcpy(e, &e2->meld[i], sizeof(Element));
        e ++;
        assert(dst->len < MJ_ELEMENTS_LEN);
        dst->len ++;
    }
}
