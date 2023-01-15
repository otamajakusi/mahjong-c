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

#if 0
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

/*
 * 食い下がり1翻
 */
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

/*
 * 食い下がり1翻
 */
int is_ittsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len - 2; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        if (is_meld_shuntsu(meld) && meld->tile_id[0] % 3 == 0 /*1,4,7*/) {
            // found first ittsu element
            uint32_t type;
            /*
             * 3で割った商は
             * m1~m3=0, m4~m6=1, m7~m9=2,
             * p1~p3=3, p4~p6=4, p7~p9=5, 
             * s1~s3=6, s4~s6=7, s7~s9=8, 
             */
            type = 1u << (meld->tile_id[0] / 3);
            for (int j = i + 1; j < merged_melds.len; j ++) {
                const Meld *target = &merged_melds.meld[j];
                if (is_meld_shuntsu(target)) {
                    type |= 1u << (target->tile_id[0] / 3);
                }
            }
            /*
             * tile_idを3で割った商で1を右シフトすると
             * m1,m2,m3,m4,m5,m6,m7,m8,m9があれば (1<<0|1<<1|1<<2)==0x0007
             * p1,p2,p3,p4,p5,p6,p7,p8,p9があれば (1<<3|1<<4|1<<5)==0x0038
             * s1,s2,s3,s4,s5,s6,s7,s8,s9があれば (1<<6|1<<7|1<<8)==0x01c0
             *
             */
            if ((type & 0x7) == 0x7 || ((type >> 3) & 0x7) == 0x7 || ((type >> 6) & 0x7) == 0x7) {
                // found three elements - 123,456,789
                return true;
            }
        }
    }
    return false;
}

/*
 * 食い下がり1翻
 * ヤオチュウ(1,9,字)牌を含む面子だけ(123の順子はOK)
 * (すべてヤオチュウ牌の手はホンロウトウ)
 */
int is_chanta(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (is_meld_shuntsu(meld)) {
            uint32_t number = tile_id % 9;
            if (number != 0 && number != 6) { // 123 or 789
                return false;
            }
        } else { // kotsu or kantsu
            if (tile_id < TON) {
                uint32_t number = tile_id % 9;
                if (number != 0 && number != 8) { // 111 or 999
                    return false;
                }
            }
        }
    }
    if (!IS_ONE(head_tile_id) || !IS_NINE(head_tile_id) || !IS_WIND(head_tile_id) || !IS_DRAGON(head_tile_id)) {
        return false;
    }
    return true;
}

/*
 * TODO
 * これはmeldで判定しない
 */
int is_chiitoitsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
}

/***** 3翻 *****/
/*
 * 二盃口, 門前, 一盃口の上位役
 * NOTE: 同種同順(m1m2m3 m1m2m3 m1m2m3 m1m2m3)でも成り立つとする
 */
int is_ryanpeiko(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    if (open_melds->len) {  // 暗槓含む副露牌がある
        return false;
    }
    int count;
    count = count_same_melds(tiles_melds);
    if (count != 6 && count != 2) {
        return false;
    }
    return true;
}

/*
 * 混一色, チンイツの下位役
 * 1種類の数牌と字牌のみ
 * 食い下がり2翻
 */
int is_honitsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    uint32_t type = 0;
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (tile_id < TON) {
            type |= 1 << (tile_id / 9);
        }
    }
    if (head_tile_id < TON) {
        type |= 1 << (head_tile_id / 9);
    }
    if (type != 1 && type != 2 && type != 4) {  // 萬子だけ==1, 筒子だけ==2, 索子だけ==4
        return false;
    }
    return true;
}

/*
 * 純全帯么九(純チャン)(字牌を含まないチャンタ)
 * 食い下がり2翻
 */
int is_junchan(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (tile_id >= TON) {
            return false;
        }
        if (is_meld_shuntsu(meld)) {
            uint32_t number = tile_id % 9;
            if (number != 0 && number != 6) { // 123 or 789
                return false;
            }
        } else { // kotsu or kantsu
            uint32_t number = tile_id % 9;
            if (number != 0 && number != 8) { // 111 or 999
                return false;
            }
        }
    }
    if (!IS_ONE(head_tile_id) || !IS_NINE(head_tile_id)) {
        return false;
    }
    return true;
}

/***** 6翻 *****/
/*
 * 清一色(チンイツ)(字牌を使わないホンイツ)
 * 食い下がり5翻
 */
int is_chinitsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    uint32_t type = 0;
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (tile_id >= TON) {
            return false;
        }
        type |= 1 << (tile_id / 9);
    }
    if (head_tile_id >= TON) {
        return false;
    }
    type |= 1 << (head_tile_id / 9);
    if (type != 1 && type != 2 && type != 4) {  // 萬子だけ==1, 筒子だけ==2, 索子だけ==4
        return false;
    }
    return true;
}

/***** 役満 *****/
/*
 * 国士無双, 門前
 * TODO
 * これはmeldで判定しない
 */
int is_kokushi(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
}

/*
 * 四暗刻, 門前
 */
int is_suuankou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        if (!meld->is_closed) {
            return false;
        }
        if (!is_meld_kotsu(meld) || !is_meld_kantsu(meld)) {
            return false;
        }
    }
    return true;
}

/*
 * 大三元
 */
int is_daisangen(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    uint32_t count = 0;
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        if (IS_DRAGON(meld->tile_id[0])) {
            count ++;
        }
    }
    if (count < 3) {
        return false;
    }
    return true;
}

/*
 * 緑一色(緑發が入っていなくてもよい)
 * 索子の23468と發
 * (68發は刻子か雀頭, 牌が6種類の為七対子はできない)
 */
int is_ryuisou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (is_meld_shuntsu(meld) && tile_id != SOU2) {
            return false;
        } else
        if (tile_id != SOU2 &&
            tile_id != SOU3 &&
            tile_id != SOU4 &&
            tile_id != SOU6 &&
            tile_id != SOU8 &&
            tile_id != HATSU) {
            return false;
        }
    }
    if (head_tile_id != SOU2 &&
        head_tile_id != SOU3 &&
        head_tile_id != SOU4 &&
        head_tile_id != SOU6 &&
        head_tile_id != SOU8 &&
        head_tile_id != HATSU) {
        return false;
    }
    return false;
}

/*
 * 字一色
 */
int is_tsuisou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (tile_id < TON) {
            return false;
        }
    }
    if (head_tile_id < TON) {
        return false;
    }
    return true;
}

/*
 * 小四喜
 */
int is_shosuushi(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    uint32_t count = 0;
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (tile_id >= TON && tile_id <= PEI) {
            count ++;
        }
    }
    if (head_tile_id >= TON && head_tile_id <= PEI) {
        count ++;
    }
    if (count != 4) {
        return false;
    }
    return true;
}
/*
 * 大四喜, 小四喜の上位役
 */
int is_daisuushi(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    uint32_t count = 0;
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (tile_id >= TON && tile_id <= PEI) {
            count ++;
        }
    }
    if (count != 4) {
        return false;
    }
    return true;
}

/*
 * 清老頭(すべて1,9牌のみで揃える,鳴きOK), ホンロウトウの上位役
 */
int is_chinroto(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    Melds merged_melds;
    merge_melds(&merged_melds, tiles_melds, open_melds);
    assert(merged_melds.len == MENTSU_LEN);
    for (int i = 0; i < merged_melds.len; i ++) {
        const Meld *meld = &merged_melds.meld[i];
        uint8_t tile_id = meld->tile_id[0];
        if (!is_meld_kotsu(meld) || !is_meld_kantsu(meld)) {
            return false;
        }
        uint32_t number = tile_id % 9;
        if (number != 0 && number != 8) {
            return false;
        }
    }
    return true;
}

/*
 * 四槓子
 */
int is_suukantsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    int count = 0;
    for (int i = 0; i < open_melds->len; i ++) {
        const Meld *meld = &open_melds->meld[i];
        if (is_meld_kantsu(meld)) {
            count ++;
        }
    }
    if (count != 4) {
        return false;
    }
    return true;
}

/*
 * 九蓮宝燈, 門前
 * 1112345678999 + x
 * 1が3枚, 2345678が1枚, 9が3枚
 * TODO:
 */
int is_chuuren_poutou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg) {
    if (open_melds->len) {
        return false;
    }
    // すべて同種牌
    // 1が3枚, 2345678が1枚, 9が3枚
}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
    if (count_elements_same_sequences(concealed_elems) == 0) {
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

/* ダブ東: 門前: 不要, 説明: 東が自風かつ場風のとき東の刻子を構成 */
int is_double_ton(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* ダブ南: 門前: 不要, 説明: 南が自風かつ場風のとき南の刻子を構成 */
int is_double_nan(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* ダブ西: 門前: 不要, 説明: 西が自風かつ場風のとき西の刻子を構成 */
int is_double_sha(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* ダブ北: 門前: 不要, 説明: 北が自風かつ場風のとき北の刻子を構成 */
int is_double_pei(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 七対子: 門前: 必要, 説明: 7種類の対子で構成. 常に25符. 同種の牌が4枚の場合は不成立. 一盃口, 二盃口と複合しない. */
int is_chiitoitsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);

/*** 2翻(食い下がり1翻) ***/
/* 三色同順: 門前: 不要, 食い下がり: 1翻, 説明: 同数異種の順子を3つ構成 */
int is_sanshoku(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 一気通貫: 門前: 不要, 食い下がり: 1翻, 説明: 同数順子で123,456,789を構成 */
int is_ittsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 混全帯么九: 門前: 不要, 食い下がり: 1翻, 説明: すべての面子と雀頭に么九牌(1,9,字牌)を含む(123はOK) */
int is_chanta(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);

/*** 3翻 ***/
/* 二盃口: 門前: 必須, 説明: 一盃口を2組を構成. 同種同順が2組でも成立. */
int is_ryanpeiko(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/*** 3翻(食い下がり2翻) ***/
/* 混一色: 門前: 不要, 食い下がり: 2翻, 説明: 同種の数牌と字牌のみで構成. */
int is_honitsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 純全帯么九: 門前: 不要, 食い下がり: 2翻, 説明: すべての面子と雀頭を老頭牌(1,9牌)を含む(123はOK). 混全帯么九に字牌が含まれない場合の構成. */
int is_junchan(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);

/*** 6翻(食い下がり5翻) ***/
/* 清一色: 門前: 不要, 食い下がり: 5翻, 説明: 同種の数牌のみで構成. */
int is_chinitsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);

/*** 役満 ***/
/* 国士無双: 門前: 必要, 説明: すべての種類の么九牌で構成される. */
int is_kokushi(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 四暗刻: 門前: 必要, 説明: 面子を暗刻(暗槓含む)で構成. 注意: ロンアガリで面子が揃う場合は明刻扱い. */
int is_suuankou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 大三元: 門前: 不要, 説明: 三元牌をすべて刻子で構成 */
int is_daisangen(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 緑一色: 門前: 不要, 説明: 索子の23468と發のいずれかで構成. 發が含まれていなくてもよい */
int is_ryuisou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 字一色: 門前: 不要, 配がすべて字牌で4面子1雀頭もしくは七対子 */
int is_tsuisou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 小四喜: 門前: 不要, 1つの風牌の刻子と風牌の雀頭で構成 */
int is_shosuushi(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 大四喜: 門前: 不要, 風牌ですべての面子を構成 */
int is_daisuushi(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 清老頭: 門前: 不要, すべて老頭牌(1,9牌)で構成. 混老頭の上位役 */
int is_chinroto(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 四槓子: 門前: 不要, 4面子を槓子で構成 */
int is_suukantsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 九蓮宝燈: 門前: 必要, 同種の数牌が1112345678999 + xで構成 */
int is_chuuren_poutou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);  // 九蓮宝燈, 門前


