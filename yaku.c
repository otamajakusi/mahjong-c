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

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "yaku.h"

/*
 * https://www.nihon-majan.org/majyankisoyougo.html
 * 中張(チュンチャン)牌: 2...8
 * 老頭(ロウトウ)牌: 1,9
 * 么九(ヤオチュウ)牌: 老頭牌と字牌
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mahjong.h"
#include "yaku.h"
#include "tile.h"


/*** 1翻 ***/
/* 平和: 門前: 必須, 説明: 役牌以外で構成, 面子を順子のみで構成し両面待ちで上がる. ロンで30符, ツモで20符 */
int is_pinfu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    if (melded_elems->len) {
        return false;
    }
    if (is_tile_id_dragon(pair_tile)) {
        return false;
    }
    if (pair_tile == cfg->player_wind || pair_tile == cfg->round_wind) {
        return false;
    }
    if (count_elements_sequence(concealed_elems) != MJ_ELEMENTS_LEN) {
        return false;
    }
    if (!is_ryanmen_machi(concealed_elems, cfg->win_tile)) {
        return false;
    }
    return true;
}

/* 断么九: 門前: 不要, 説明: 么九牌以外で構成 */
int is_tanyao(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    if (is_tile_id_dragon(pair_tile)) {
        return false;
    }
    if (pair_tile == cfg->player_wind || pair_tile == cfg->round_wind) {
        return false;
    }
    if (!is_elements_chunchan(concealed_elems)) {
        return false;
    }
    if (!is_elements_chunchan(melded_elems)) {
        return false;
    }
    return true;
}

/* 一盃口: 門前: 必須, 説明: 同数同種の数牌の順子を2組を構成 */
int is_iipeiko(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    if (has_elements_melded(melded_elems)) {
        return false;
    }
    if (count_elements_same_sequence(concealed_elems) == 0) {
        return false;
    }
    return true;
}

/* 白: 門前: 不要, 説明: 白の刻子を構成 */
int is_haku(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    if (has_elements_tile_id(concealed_elems, MJ_DW) || has_elements_tile_id(melded_elems, MJ_DW)) {
        return true;
    }
    return false;
}

/* 發: 門前: 不要, 説明: 發の刻子を構成 */
int is_hatsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    if (has_elements_tile_id(concealed_elems, MJ_DG) || has_elements_tile_id(melded_elems, MJ_DG)) {
        return true;
    }
    return false;
}

/* 中: 門前: 不要, 説明: 中の刻子を構成 */
int is_chun(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    if (has_elements_tile_id(concealed_elems, MJ_DR) || has_elements_tile_id(melded_elems, MJ_DR)) {
        return true;
    }
    return false;
}

static int is_wind(const Elements *concealed_elems, const Elements *melded_elems, const ScoreConfig *cfg, MJTileId wind) {
    if (cfg->player_wind != wind && cfg->round_wind != wind) {
        return false;
    }
    if (has_elements_tile_id(concealed_elems, wind) || has_elements_tile_id(melded_elems, wind)) {
        return true;
    }
    return false;
}

/* 東: 門前: 不要, 説明: 東が役牌のとき東の刻子を構成 */
int is_ton(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    return is_wind(concealed_elems, melded_elems, cfg, MJ_WT);
}

/* 南: 門前: 不要, 説明: 南が役牌のとき南の刻子を構成 */
int is_nan(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    return is_wind(concealed_elems, melded_elems, cfg, MJ_WN);
}

/* 西: 門前: 不要, 説明: 西が役牌のとき西の刻子を構成 */
int is_sha(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    return is_wind(concealed_elems, melded_elems, cfg, MJ_WS);
}

/* 北: 門前: 不要, 説明: 北が役牌のとき北の刻子を構成 */
int is_pei(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    return is_wind(concealed_elems, melded_elems, cfg, MJ_WP);
}

/*** 2翻 ***/
/* 対々和: 門前: 不要, 説明: 面子を刻子のみで構成 */
int is_toitoi(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    uint32_t count = 0;
    count += count_elements_triplets(concealed_elems);
    count += count_elements_triplets(melded_elems);
    count += count_elements_fours(melded_elems);
    return count == MJ_ELEMENTS_LEN;
}

/* 三暗刻: 門前: 不要, 説明: 暗刻を3つ構成 */
int is_sanankou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    uint32_t count = 0;
    count += count_elements_concealed_fours(melded_elems);
    count += count_elements_triplets(concealed_elems);
    if (count < 3) {
        return false;
    }
    /* 刻子が3枚 && ロンアガリ && シャンポン待ち => 三暗刻不成立 */
    if (count == 3 && cfg->ron && is_shanpon_machi(concealed_elems, cfg->win_tile)) {
        return false;
    }
    return true;
}

/* 三色同刻: 門前: 不要, 説明: 同数異種の刻子を3つ構成 */
int is_sanshoku_douko(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    for (uint32_t i = 0; i < merged_elems.len - 2; i ++) {
        const Element *elem = &merged_elems.meld[i];
        if ((is_element_triplets(elem) || is_element_fours(elem))) {
            if (is_tile_id_honors(elem->tile_id[0])) {
                continue;
            }
            // found first triplets or fours 数牌
            uint32_t type;
            uint32_t number;
            type = get_tile_type(elem->tile_id[0]);
            number = get_tile_number(elem->tile_id[0]);
            for (uint32_t j = i + 1; j < merged_elems.len; j ++) {
                const Element *target = &merged_elems.meld[j];
                if (is_element_triplets(target) || is_element_fours(target)) {
                    if (is_tile_id_honors(target->tile_id[0])) {
                        continue;
                    }
                    uint32_t target_number = get_tile_number(target->tile_id[0]);
                    if (number == target_number) {
                        type |= get_tile_type(target->tile_id[0]);
                    }
                }
            }
            if (type == (TILE_TYPE_MAN | TILE_TYPE_PIN | TILE_TYPE_SOU)) {
                // found three types - man, pin and sou
                return true;
            }
        }
    }
    return false;
}

/* 三槓子: 門前: 不要, 説明: 槓子を3つ構成 */
int is_sankantsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)concealed_elems;
    (void)pair_tile;
    (void)cfg;
    uint32_t count;
    count = count_elements_fours(melded_elems);
    return count >= 3;
}

/* 小三元: 門前: 不要, 説明: 三元牌を2つ刻子, 1つ雀頭で構成 */
int is_shosangen(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    bool haku = has_elements_tile_id(concealed_elems, MJ_DW) || has_elements_tile_id(melded_elems, MJ_DW);
    bool hatsu = has_elements_tile_id(concealed_elems, MJ_DG) || has_elements_tile_id(melded_elems, MJ_DG);
    bool chun = has_elements_tile_id(concealed_elems, MJ_DR) || has_elements_tile_id(melded_elems, MJ_DR);
    if ((!haku && !hatsu) ||
        (!haku && !chun) ||
        (!hatsu && !chun)) {
        return false;
    }
    /* もし刻子が白撥中のどれか2つでできていたら, 雀頭は刻子で構成された白撥中と別の牌で構成される */
    if (pair_tile != MJ_DW && pair_tile != MJ_DG && pair_tile != MJ_DR) {
        return false;
    }
    return true;
}

/* 混老頭: 門前: 不要, 説明: 么九牌(1,9, 字牌)だけで構成(七対子もしくは対々和と必ず複合する) */
/* TODO: 七対子対応 */
int is_honroto(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    if (!is_elements_yaochu(concealed_elems)) {
        return false;
    }
    if (!is_elements_yaochu(melded_elems)) {
        return false;
    }
    if (!is_tile_id_yaochu(pair_tile)) {
        return false;
    }
    return true;
}

static int is_double_wind(const Elements *concealed_elems, const Elements *melded_elems, const ScoreConfig *cfg, MJTileId wind) {
    if (cfg->player_wind != wind || cfg->round_wind != wind) {
        return false;
    }
    if (has_elements_tile_id(concealed_elems, wind) || has_elements_tile_id(melded_elems, wind)) {
        return true;
    }
    return false;
}

/* ダブ東: 門前: 不要, 説明: 東が自風かつ場風のとき東の刻子を構成 */
int is_double_ton(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    return is_double_wind(concealed_elems, melded_elems, cfg, MJ_WT);
}

/* ダブ南: 門前: 不要, 説明: 南が自風かつ場風のとき南の刻子を構成 */
int is_double_nan(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    return is_double_wind(concealed_elems, melded_elems, cfg, MJ_WN);
}

/* ダブ西: 門前: 不要, 説明: 西が自風かつ場風のとき西の刻子を構成 */
int is_double_sha(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    return is_double_wind(concealed_elems, melded_elems, cfg, MJ_WS);
}

/* ダブ北: 門前: 不要, 説明: 北が自風かつ場風のとき北の刻子を構成 */
int is_double_pei(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    return is_double_wind(concealed_elems, melded_elems, cfg, MJ_WP);
}

/* 七対子: 門前: 必要, 説明: 7種類の対子で構成. 常に25符. 同種の牌が4枚の場合は不成立. 一盃口, 二盃口と複合しない. */
/* TODO */
int is_chiitoitsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);

/*** 2翻(食い下がり1翻) ***/
/* 三色同順: 門前: 不要, 食い下がり: 1翻, 説明: 同数異種の順子を3つ構成 */
int is_sanshoku(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    for (uint32_t i = 0; i < merged_elems.len - 2; i ++) {
        const Element *elem = &merged_elems.meld[i];
        if (is_element_sequence(elem)) {
            // found first sequence
            uint32_t type;
            uint32_t number;
            type = get_tile_type(elem->tile_id[0]);
            number = get_tile_number(elem->tile_id[0]);
            for (uint32_t j = i + 1; j < merged_elems.len; j ++) {
                const Element *target = &merged_elems.meld[j];
                if (is_element_sequence(target)) {
                    uint32_t target_number = get_tile_number(target->tile_id[0]);
                    if (number == target_number) {
                        type |= get_tile_type(target->tile_id[0]);
                    }
                }
            }
            if (type == (TILE_TYPE_MAN | TILE_TYPE_PIN | TILE_TYPE_SOU)) {
                // found three types - man, pin and sou
                return true;
            }
        }
    }
    return false;
}

/* 一気通貫: 門前: 不要, 食い下がり: 1翻, 説明: 同数順子で123,456,789を構成 */
int is_ittsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    for (uint32_t i = 0; i < merged_elems.len - 2; i ++) {
        const Element *elem = &merged_elems.meld[i];
        if (is_element_sequence(elem)) {
            uint32_t number;
            number = get_tile_number(elem->tile_id[0]);
            if (number % 3 != 0) { // search 0,3,6(=1,4,7)
                continue;
            }
            uint32_t area; // 1: 123 area, 2: 456 area, 4: 789 area.
            area = 1u << (number / 3);
            // found first ittsu element
            uint32_t type;
            type = get_tile_type(elem->tile_id[0]);
            for (uint32_t j = i + 1; j < merged_elems.len; j ++) {
                const Element *target = &merged_elems.meld[j];
                if (is_element_sequence(target)) {
                    uint32_t target_number = get_tile_number(target->tile_id[0]);
                    if (target_number % 3 != 0) { // search 0,3,6(=1,4,7)
                        continue;
                    }
                    if (type != get_tile_type(target->tile_id[0])) {
                        continue;
                    }
                    area = 1u << (target_number / 3);
                }
            }
            if (area == 7) {
                return true;
            }
        }
    }
    return false;
}

/* 混全帯么九: 門前: 不要, 食い下がり: 1翻, 説明: すべての面子と雀頭に么九牌(1,9,字牌)を含む(123はOK) */
int is_chanta(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    if (!has_elements_yaochu(&merged_elems)) {
        return false;
    }
    if (!is_tile_id_yaochu(pair_tile)) {
        return false;
    }
    return true;
}

/*** 3翻 ***/
/* 二盃口: 門前: 必須, 説明: 一盃口を2組を構成. 同種同順が2組でも成立. */
int is_ryanpeiko(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    if (has_elements_melded(melded_elems)) {
        return false;
    }
    uint32_t count = count_elements_same_sequence(concealed_elems);
    if (count != 6 && count != 2) {
        return false;
    }
    return true;
}

/*** 3翻(食い下がり2翻) ***/
/* 混一色: 門前: 不要, 食い下がり: 2翻, 説明: 同種の数牌と字牌のみで構成. */
int is_honitsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    uint32_t type = 0;
    for (uint32_t i = 0; i < merged_elems.len; i ++) {
        const Element *elem = &merged_elems.meld[i];
        if (is_tile_id_honors(elem->tile_id[0])) {
            continue;
        }
        type |= get_tile_type(elem->tile_id[0]);
    }
    type |= get_tile_type(pair_tile);
    /* has only one type */
    if (type != TILE_TYPE_MAN && type != TILE_TYPE_PIN && type != TILE_TYPE_SOU) {
        return false;
    }
    return true;
}

/* 純全帯么九: 門前: 不要, 食い下がり: 2翻, 説明: すべての面子と雀頭を老頭牌(1,9牌)を含む(123はOK). 混全帯么九に字牌が含まれない場合の構成. */
int is_junchan(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    if (!has_elements_routou(&merged_elems)) {
        return false;
    }
    uint32_t number = get_tile_number(pair_tile);
    if (number != 0 && number != 8) {
        return false;
    }
    return true;
}

/*** 6翻(食い下がり5翻) ***/
/* 清一色: 門前: 不要, 食い下がり: 5翻, 説明: 同種の数牌のみで構成. */
/* TODO: 七対子 */
int is_chinitsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    uint32_t type = 0;
    for (uint32_t i = 0; i < merged_elems.len; i ++) {
        const Element *elem = &merged_elems.meld[i];
        type |= get_tile_type(elem->tile_id[0]);
    }
    type |= get_tile_type(pair_tile);
    /* has only one type */
    if (type != TILE_TYPE_MAN && type != TILE_TYPE_PIN && type != TILE_TYPE_SOU) {
        return false;
    }
    return true;
}
/*** 役満 ***/
/* 国士無双: 門前: 必要, 説明: すべての種類の么九牌で構成される. */
/* TODO */
int is_kokushi(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);

/* 四暗刻: 門前: 必要, 説明: 面子を暗刻(暗槓含む)で構成. 注意: ロンアガリで面子が揃う場合は明刻扱い. */
int is_suuankou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    uint32_t count = 0;
    count += count_elements_concealed_fours(melded_elems);
    count += count_elements_triplets(concealed_elems);
    if (count < 4) {
        return false;
    }
    /* 刻子が4枚 && ロンアガリ && シャンポン待ち => 四暗刻不成立 */
    if (count == 4 && cfg->ron && is_shanpon_machi(concealed_elems, cfg->win_tile)) {
        return false;
    }
    return true;
}

/* 大三元: 門前: 不要, 説明: 三元牌をすべて刻子で構成 */
int is_daisangen(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    bool haku = has_elements_tile_id(concealed_elems, MJ_DW) || has_elements_tile_id(melded_elems, MJ_DW);
    bool hatsu = has_elements_tile_id(concealed_elems, MJ_DG) || has_elements_tile_id(melded_elems, MJ_DG);
    bool chun = has_elements_tile_id(concealed_elems, MJ_DR) || has_elements_tile_id(melded_elems, MJ_DR);
    if (!haku || !hatsu || !chun) {
        return false;
    }
    return true;
}

/* 緑一色: 門前: 不要, 説明: 索子の23468と發のいずれかで構成. 發が含まれていなくてもよい */
int is_ryuisou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    for (uint32_t i = 0; i < merged_elems.len; i ++) {
        const Element *elem = &merged_elems.meld[i];
        MJTileId tile_id = elem->tile_id[0];
        if (is_element_sequence(elem) && tile_id != MJ_S2) {
            return false;
        } else {
            if (tile_id != MJ_S2 &&
                tile_id != MJ_S3 &&
                tile_id != MJ_S4 &&
                tile_id != MJ_S6 &&
                tile_id != MJ_S8 &&
                tile_id != MJ_DG) {
                return false;
            }
        }
    }
    if (pair_tile != MJ_S2 &&
        pair_tile != MJ_S3 &&
        pair_tile != MJ_S4 &&
        pair_tile != MJ_S6 &&
        pair_tile != MJ_S8 &&
        pair_tile != MJ_DG) {
        return false;
    }
    return true;
}

/* 字一色: 門前: 不要, 配がすべて字牌で4面子1雀頭もしくは七対子 */
int is_tsuisou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    for (uint32_t i = 0; i < merged_elems.len; i ++) {
        const Element *elem = &merged_elems.meld[i];
        if (!is_tile_id_honors(elem->tile_id[0])) {
            return false;
        }
    }
    if (!is_tile_id_honors(pair_tile)) {
        return false;
    }
    return true;
}

/* 小四喜: 門前: 不要, 1つの風牌の刻子と風牌の雀頭で構成 */
int is_shosuushi(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    uint32_t count = 0;
    for (uint32_t i = 0; i < merged_elems.len; i ++) {
        const Element *elem = &merged_elems.meld[i];
        if (is_tile_id_wind(elem->tile_id[0])) {
            count ++;
        }
    }
    if (!is_tile_id_wind(pair_tile)) {
        count ++;
    }
    if (count < 4) {
        return false;
    }
    return true;
}

/* 大四喜: 門前: 不要, 風牌ですべての面子を構成 */
int is_daisuushi(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)pair_tile;
    (void)cfg;
    Elements merged_elems;
    merge_elements(&merged_elems, concealed_elems, melded_elems);
    assert(merged_elems.len == MJ_ELEMENTS_LEN);
    uint32_t count = 0;
    for (uint32_t i = 0; i < merged_elems.len; i ++) {
        const Element *elem = &merged_elems.meld[i];
        if (is_tile_id_wind(elem->tile_id[0])) {
            count ++;
        }
    }
    if (count < 4) {
        return false;
    }
    return true;
}

/* 清老頭: 門前: 不要, すべて老頭牌(1,9牌)で構成. 混老頭の上位役 */
int is_chinroto(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)cfg;
    if (!is_elements_routou(concealed_elems)) {
        return false;
    }
    if (!is_elements_routou(melded_elems)) {
        return false;
    }
    if (!is_tile_id_routou(pair_tile)) {
        return false;
    }
    return true;
}

/* 四槓子: 門前: 不要, 4面子を槓子で構成 */
int is_suukantsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg) {
    (void)concealed_elems;
    (void)pair_tile;
    (void)cfg;
    return count_elements_fours(melded_elems) == 4;
}

/* 九蓮宝燈: 門前: 必要, 同種の数牌が1112345678999 + xで構成 */
/* TODO */
int is_chuuren_poutou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);  // 九蓮宝燈, 門前


