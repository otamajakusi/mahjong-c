#include <stdio.h>
#include <assert.h>

#include "test_element.h"
#include "test_util.h"

bool gen_elements_from_melds(Elements *elems, const MJMelds *melds);

bool is_element_sequence(const Element *elem);
bool is_element_triplet(const Element *elem);
bool is_element_fours(const Element *elem);

/* single element */
/* 2..8 */
bool is_element_chunchan(const Element *elem);
/* 1 or 9 */
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

/*
 * counts same sequence element in elements
 * 123, 123, 123, 123 => 6
 * 123, 123, 123, xxx => 3
 * 123, 123, yyy, xxx => 1
 * 123, zzz, yyy, xxx => 0
 * 123, 456, 123, 456 => 2
 */
uint32_t count_elements_sequence(const Elements *elems);
uint32_t count_elements_triplets(const Elements *elems);
uint32_t count_elements_fours(const Elements *elems);
uint32_t count_elements_concealed_fours(const Elements *elems);
bool has_elements_melded(const Elements *elems); /* 暗槓はconcealed扱い */

bool is_ryanmen_machi(const Elements *elems, MJTileId win_tile);
bool is_shanpon_machi(const Elements *elems, MJTileId win_tile);
bool is_tanki_machi(MJTileId pair_tile, MJTileId win_tile);
uint32_t count_elements_same_sequences(const Elements *elems);

bool has_elements_tile_id(const Elements *elems, MJTileId tile_id);
void merge_elements(Elements *dst, const Elements *e1, const Elements *e2);

bool test_element() {
    return true;
}
