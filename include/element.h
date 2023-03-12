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

typedef MJMeld Element;
typedef MJMelds Elements;

typedef enum {
  ELEM_TYPE_SEQUENCE,
  ELEM_TYPE_TRIPLETS,
  ELEM_TYPE_FOURS,
} ElementType;

/*
 * melds should be checked with is_valid_melds befor hands.
 */
bool gen_elements_from_melds(Elements *elems, const MJMelds *melds);

/*
 * Elements or Element arguments of the following functions are assumed to be generated with gen_elements_from_melds.
 */

/* single element */
/* 2..8 */
bool is_element_chunchan(const Element *elem);
/* only 111 or 999 */
bool is_element_routou(const Element *elem);
/* 123, 789, 111, 999 */
bool has_element_routou(const Element *elem);
/* 1,9,字牌 */
bool is_element_yaochu(const Element *elem);
/* 123, 789, 111, 999 or 字牌 */
bool has_element_yaochu(const Element *elem);

bool is_element_sequence(const Element *elem);
bool is_element_triplets(const Element *elem);
bool is_element_fours(const Element *elem);
bool is_element_concealed(const Element *elem);

/* all elements */
bool is_elements_chunchan(const Elements *elems);
bool is_elements_routou(const Elements *elems);
bool has_elements_routou(const Elements *elems);
bool is_elements_yaochu(const Elements *elems);
bool has_elements_yaochu(const Elements *elems);
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
bool is_kanchan_machi(const Elements *elems, MJTileId win_tile);
bool is_penchan_machi(const Elements *elems, MJTileId win_tile);
bool is_shanpon_machi(const Elements *elems, MJTileId win_tile);
bool is_tanki_machi(MJTileId pair_tile, MJTileId win_tile);
/*
 * counts same sequence element in elements
 * 123, 123, 123, 123 => 6
 * 123, 123, 123, xxx => 3
 * 123, 123, yyy, xxx => 1
 * 123, zzz, yyy, xxx => 0
 * 123, 456, 123, 456 => 2
 */
uint32_t count_elements_same_sequence(const Elements *elems);

/*
 * compare if e1 and e2 has same elements
 * e.g.
 * e1 = {{{{m1,m2,m3},3,true},{{s1,s2,s3},3,true},{{m4,m5,m6},3,true}}, 3};
 * e2 = {{{{m4,m5,m6},3,true},{{s1,s2,s3},3,true},{{m1,m2,m3},3,true}}, 3};
 * e1 and e2 are the same.
 */
bool is_same_element(const Element *e1, const Element *e2);
bool is_same_elements(const Elements *e1, const Elements *e2);

bool has_elements_tile_id(const Elements *elems, MJTileId tile_id);
void merge_elements(Elements *dst, const Elements *e1, const Elements *e2);
