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
 * チュンチャン(中張)牌: 2...8
 * ロウトウ(老頭)牌: 1,9
 * ヤオチュウ(么九)牌: ロウトウ牌と字牌
 *
 */

/*
 * Meld
 */
static int is_meld_chunchan(const Meld *meld) {
    for (int i = 0; i < meld->len; i ++) {
        uint8_t tile_id = meld->tile_id[i];
        if (IS_WIND(tile_id)) {
            return false;
        }
        if (IS_DRAGON(tile_id)) {
            return false;
        }
        if (IS_ONE(tile_id)) {
            return false;
        }
        if (IS_NINE(tile_id)) {
            return false;
        }
    }
    return true;
}

/* 1,9のみを含んだmeld(e.g. 111, 999)ならtrue */
static int is_meld_routou(const Meld *meld) {
    for (int i = 0; i < meld->len; i ++) {
        uint8_t tile_id = meld->tile_id[i];
        if (IS_WIND(tile_id)) {
            return false;
        }
        if (IS_DRAGON(tile_id)) {
            return false;
        }
        if (!IS_ONE(tile_id) || !IS_NINE(tile_id)) {
            return false;
        }
    }
    return true;
}

/* 1,9,字牌を含んだmeld(e.g. 123, 789)ならtrue */
static int has_meld_yaochu(const Meld *meld) {
    return !is_meld_chunchan(meld);
}

/* 1,9,字牌のみを含んだmeldならtrue */
static int is_meld_yaochu(const Meld *meld) {
    for (int i = 0; i < meld->len; i ++) {
        uint8_t tile_id = meld->tile_id[i];
        if (!IS_WIND(tile_id) || !IS_DRAGON(tile_id) || !IS_ONE(tile_id) || !IS_NINE(tile_id)) {
            return false;
        }
    }
    return true;
}

/* meldは適切な面子(e.g 231, 999)であることは確認されている */
static int is_meld_shuntsu(const Meld *meld) {
    if (meld->len != 3) {
        return false;
    }
    if (meld->tile_id[0] == meld->tile_id[1] || 
        meld->tile_id[1] == meld->tile_id[2] ||
        meld->tile_id[2] == meld->tile_id[0]) {
        return false;
    }
    return true;
}

/* meldは適切な面子(e.g 231, 999)であることは確認されている */
static int is_meld_kotsu(const Meld *meld) {
    if (meld->len != 3) {
        return false;
    }
    if (meld->tile_id[0] != meld->tile_id[1] || 
        meld->tile_id[1] != meld->tile_id[2] ||
        meld->tile_id[2] != meld->tile_id[0]) {
        return false;
    }
    return true;
}

static int is_meld_kantsu(const Meld *meld) {
    if (meld->len != 4) {
        return false;
    }
    if (meld->tile_id[0] != meld->tile_id[1] || 
        meld->tile_id[1] != meld->tile_id[2] ||
        meld->tile_id[2] != meld->tile_id[3] ||
        meld->tile_id[3] != meld->tile_id[0]) {
        return false;
    }
    return true;
}

static int is_meld_closed(const Meld *meld) {
    return meld->is_closed;
}

/*
 * Melds
 */
static int is_melds_chunchan(const Melds *melds) {
    for (int i = 0; i < melds->len; i ++) {
        if (!is_meld_chunchan(&melds->meld[i])) {
            return false;
        }
    }
    return true;
}

static int is_melds_routou(const Melds *melds) {
    for (int i = 0; i < melds->len; i ++) {
        if (!is_meld_routou(&melds->meld[i])) {
            return false;
        }
    }
    return true;
}

static int has_melds_yaochu(const Melds *melds) {
    for (int i = 0; i < melds->len; i ++) {
        if (!has_meld_yaochu(&melds->meld[i])) {
            return false;
        }
    }
    return true;
}

static int is_melds_yaochu(const Melds *melds) {
    for (int i = 0; i < melds->len; i ++) {
        if (!is_meld_yaochu(&melds->meld[i])) {
            return false;
        }
    }
    return true;
}

static int count_melds_shuntsu(const Melds *melds) {
    int count = 0;
    for (int i = 0; i < melds->len; i ++) {
       if (is_meld_shuntsu(&melds->meld[i])) {
           count ++;
       }
    }
    return count;
}

static int count_melds_kotsu(const Melds *melds) {
    int count = 0;
    for (int i = 0; i < melds->len; i ++) {
       if (is_meld_kotsu(&melds->meld[i])) {
           count ++;
       }
    }
    return count;
}

static int count_melds_kantsu_closed(const Melds *melds) {
    int count = 0;
    for (int i = 0; i < melds->len; i ++) {
       if (is_meld_kantsu(&melds->meld[i]) && is_meld_closed(&melds->meld[i])) {
           count ++;
       }
    }
    return count;
}

static int count_melds_kantsu(const Melds *melds) {
    int count = 0;
    for (int i = 0; i < melds->len; i ++) {
       if (is_meld_kantsu(&melds->meld[i])) {
           count ++;
       }
    }
    return count;
}

/* 副露のmeldsが1つでも含まれていればtrue */
static int has_melds_open(const Melds *melds) {
    for (int i = 0; i < melds->len; i ++) {
       if (!melds->meld[i].is_closed) {
           return true;
       }
    }
    return false;
}

/* meldsは順子であることを事前に確認すること */
static int is_ryanmen_machi(const Melds *melds, uint8_t win_tile) {
    for (int i = 0; i < melds->len; i ++) {
        const Meld *meld = &melds->meld[i];
        if (meld->tile_id[0] == win_tile && !IS_SEVEN(win_tile)) { // n,n+1,n+2の順子でアガリ牌nが7でなければ両面待ち
            return true;
        }
        if (meld->tile_id[2] == win_tile && !IS_THREE(win_tile)) { // n,n+1,n+2の順子でアガリ牌n+2が3でなければ両面待ち
            return true;
        }
    }
    return false;
}

/*
 * 単騎待ちと両面待ちは同時に成り立つ
 * 78999, アガリ9 -> 78 99 なら6,9の両面待ち, 789 9 なら9の単騎待ち
 * 55667788, アガリ6 -> 55 678 678 なら6,9の両面待ち, 567 567 88 ならカンチャン待ち
 * -> このケースは頭が変わるので別のアガリ型として認識される
 */
static int is_tanki_machi(uint8_t head_tile_id, uint8_t win_tile) {
    return head_tile_id == win_tile;
}

static int is_same_meld(const Meld *m1, const Meld *m2) {
    if (m1->len != m2->len) {
        return false;
    }
    for (int i = 0; i < m1->len; i ++) {
        if (m1->tile_id[i] != m2->tile_id[i]) {
            return false;
        }
    }
    return true;
}

static int count_same_melds_from(uint32_t start, const Melds *melds) {
    int count = 0;
    assert(start < melds->len);
    const Meld *s = &melds->meld[start];
    for (int i = start + 1; i < melds->len; i ++) {
        if (is_same_meld(s, &melds->meld[i])) {
            count ++;
        }
    }
    return count;
}

/* TODO:
 * 123 123 123 xxxの場合を3を返す -> イーペーコー(三連刻はローカル役)
 * 123 123 123 123の場合を6を返す -> リャンペーコー
 * 123 123 456 456の場合を2を返す -> リャンペーコー
 * ->奇数ならイーペーコー?, 偶数ならリャンペーコー?
 **/
static int count_same_melds(const Melds *melds) {
    // melds->len is gt or eq 2 
    int count = 0;
    for (int i = 0; i < melds->len - 1; i ++) {
        count += count_same_melds_from(i, melds);
    }
    return count;
}

static int find_tile_id_meld(const Meld *meld, uint8_t tile_id) {
    for (int i = 0; i < meld->len; i ++) {
        if (meld->tile_id[i] == tile_id) {
            return true;
        }
    }
    return false;
}

static int find_tile_id_melds(const Melds *melds, uint8_t tile_id) {
    for (int i = 0; i < melds->len; i ++) {
        if (find_tile_id_meld(&melds->meld[i], tile_id)) {
            return true;
        }
    }
    return false;
}

/* merge s1 and s3 melds to dst */
static int merge_melds(Melds *dst, const Melds *s1, const Melds *s2) {
    dst->len = 0;
    Meld *d = &dst->meld[0];
    for (int i = 0; i < s1->len; i ++) {
        memcpy(d, &s1->meld[i], sizeof(Meld));
        d ++;
        dst->len ++;
    }
    for (int i = 0; i < s2->len; i ++) {
        memcpy(d, &s2->meld[i], sizeof(Meld));
        d ++;
        dst->len ++;
    }
}

/*
 * m1,m2,m3 と p1,p2,p3はtrue
 * m1,m1,m1 と p1,p1,p1はtrue
 * m1,m1,m1,m1 と p1,p1,p1,p1はtrue
 * m1,m2,m3 と m1,m2,m3はfalse (same type)
 */
static int is_meld_same_number_diff_type(const Meld *m1, const Meld *m2) {
    if (m1->len != m2->len) {
        return false;
    }
    for (int i = 0; i < m1->len; i ++) {
        if (m1->tile_id[i] >= TON) {
            return false;
        }
    }
    for (int i = 0; i < m2->len; i ++) {
        if (m2->tile_id[i] >= TON) {
            return false;
        }
    }
    for (int i = 0; i < m1->len; i ++) {  // m1, m2はtile_idでソート済みの想定
        if (m1->tile_id[i] == m2->tile_id[i]) {
            return false;
        }
        // here, different tile_id
        int delta;
        if (m1->tile_id[i] > m2->tile_id[i]) {
            delta = m1->tile_id[i] - m2->tile_id[i];
        } else {
            delta = m2->tile_id[i] - m1->tile_id[i];
        }
        if (delta != 9 && delta != 18) {
            return false;
        }
    }
    return true;
}

// 1翻
int is_pinfu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    if (open_melds->len) {  // 副露牌(or 暗槓)がある
        return false;
    }
    if (IS_DRAGON(head_tile_id)) {  // 雀頭が三元牌
        return false;
    }
    if (head_tile_id == cfg->player_wind || head_tile_id == cfg->round_wind) {  // 雀頭が自風or場風
        return false;
    }
    if (count_melds_shuntsu(tiles_melds) != MENTSU_LEN) {
        return false;
    }

    if (!is_ryanmen_machi(tiles_melds, cfg->win_tile)) {
        return false;
    }
    return true;
}

int is_tanyao(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    if (IS_DRAGON(head_tile_id)) {  // 雀頭が三元牌
        return false;
    }
    if (head_tile_id == cfg->player_wind || head_tile_id == cfg->round_wind) {  // 雀頭が自風or場風
        return false;
    }
    if (!is_melds_chunchan(open_melds)) {
        return false;
    }
    if (!is_melds_chunchan(tiles_melds)) {
        return false;
    }
    return true;
}

/* TODO: リャンペーコーは同種同順でも成り立つとする */
int is_iipeiko(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    if (has_melds_open(open_melds)) {  // 副露牌がある(暗槓は含まない)
        return false;
    }
    if (tiles_melds->len < 2) { // 少なくとも2面子は必要
        return false;
    }
    if (count_same_melds(tiles_melds) == 0) {
        return false;
    }
    return true;
}

static int is_dragon(const Melds *tiles_melds, const Melds *open_melds, uint8_t dragon_id) {
    if (find_tile_id_melds(open_melds, dragon_id)) {
        return true;
    }
    if (find_tile_id_melds(tiles_melds, dragon_id)) {
        return true;
    }
    return false;
}

int is_haku(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_dragon(tiles_melds, open_melds, HAKU);
}

int is_hatsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_dragon(tiles_melds, open_melds, HAKU);
}

int is_chun(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_dragon(tiles_melds, open_melds, HAKU);
}

static int is_wind(const Melds *tiles_melds, const Melds *open_melds, const ScoreConfig *cfg, uint8_t wind_id) {
    if (cfg->player_wind != wind_id && cfg->round_wind != wind_id) {
        return false;
    }
    if (find_tile_id_melds(open_melds, wind_id)) {
        return true;
    }
    if (find_tile_id_melds(tiles_melds, wind_id)) {
        return true;
    }
    return false;
}

int is_ton(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_wind(tiles_melds, open_melds, cfg, TON);
}

int is_nan(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_wind(tiles_melds, open_melds, cfg, NAN);
}

int is_sha(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_wind(tiles_melds, open_melds, cfg, SHA);
}

int is_pei(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_wind(tiles_melds, open_melds, cfg, PEI);
}

// 2翻
int is_toitoi(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    int count = 0;
    count += count_melds_kotsu(open_melds);
    count += count_melds_kantsu(open_melds);
    count += count_melds_kotsu(tiles_melds);
    return count == MENTSU_LEN;
}

int is_sanankou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    // FIXME: ロンで作られた刻子は明刻扱い->三暗刻不成立
    int count = 0;
    count += count_melds_kantsu_closed(open_melds);
    count += count_melds_kotsu(tiles_melds);
    return count >= 3;
}

int is_sanshoku_douko(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len - 2; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        if (is_meld_kotsu(meld)) {
            // found first kotsu
            uint32_t type;
            type = get_tile_type(meld->tile_id[0]);
            for (int j = i + 1; j < merged_melds.len; j ++) {
                const Meld *target = &merged_melds.meld[j];
                if (is_meld_kotsu(target)) {
                    if (is_meld_same_number_diff_type(meld, target)) {
                        type |= get_tile_type(target->tile_id[0]);
                    }
                }

            }
            if (type == (TYPE_MAN | TYPE_PIN | TYPE_SOU)) {
                // found three types - man, pin and sou
                return true;
            }
        }
    }
    return false;
}

int is_sankantsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    int count = 0;
    count += count_melds_kantsu(open_melds);
    return count >= 3;
}

int is_shosangen(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    int has_haku = is_dragon(tiles_melds, open_melds, HAKU);
    int has_hatsu = is_dragon(tiles_melds, open_melds, HATSU);
    int has_chun = is_dragon(tiles_melds, open_melds, CHUN);
    if ((!has_haku && !has_hatsu) ||
        (!has_haku && !has_chun) ||
        (!has_hatsu && !has_chun)) {
        return false;
    }
    /* もし刻子が白撥中のどれか2つでできていたら, 雀頭は刻子で構成された白撥中と別の牌で構成される */
    if (head_tile_id != HAKU || head_tile_id != HATSU || head_tile_id != CHUN) {
        return false;
    }
    return true;
}

/* 么九牌のみで構成される
 * 必ずトイトイか七対子と複合する
 * チャンタ、純チャンと複合しない
 */
int is_honroto(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    if (!is_melds_yaochu(open_melds)) {
        return false;
    }
    if (!is_melds_yaochu(tiles_melds)) {
        return false;
    }
    return true;
}

static int is_double_wind(const Melds *tiles_melds, const Melds *open_melds, const ScoreConfig *cfg, uint8_t wind_id) {
    if (cfg->player_wind != wind_id || cfg->round_wind != wind_id) {
        return false;
    }
    if (find_tile_id_melds(open_melds, wind_id)) {
        return true;
    }
    if (find_tile_id_melds(tiles_melds, wind_id)) {
        return true;
    }
    return false;
}

int is_double_ton(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_double_wind(tiles_melds, open_melds, cfg, TON);
}

int is_double_nan(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_double_wind(tiles_melds, open_melds, cfg, NAN);
}

int is_double_sha(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_double_wind(tiles_melds, open_melds, cfg, SHA);
}

int is_double_pei(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    return is_double_wind(tiles_melds, open_melds, cfg, PEI);
}

// 以下食い下がり1翻
int is_sanshoku(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len - 2; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        if (is_meld_shuntsu(meld)) {
            // found first shuntsu
            uint32_t type;
            type = get_tile_type(meld->tile_id[0]);
            for (int j = i + 1; j < merged_melds.len; j ++) {
                const Meld *target = &merged_melds.meld[j];
                if (is_meld_shuntsu(target)) {
                    if (is_meld_same_number_diff_type(meld, target)) {
                        type |= get_tile_type(target->tile_id[0]);
                    }
                }

            }
            if (type == (TYPE_MAN | TYPE_PIN | TYPE_SOU)) {
                // found three types - man, pin and sou
                return true;
            }
        }
    }
    return false;
}

int is_ittsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
}

int is_chanta(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
}

int is_chiitoitsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
}


