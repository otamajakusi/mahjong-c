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
static int is_meld_shuntu(const Meld *meld) {
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

static int is_melds_shuntu(const Melds *melds) {
    for (int i = 0; i < melds->len; i ++) {
       if (!is_meld_shuntu(&melds->meld[i])) {
           return false;
       }
    }
    return true;
}

static int is_melds_kotsu(const Melds *melds) {
}

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
    if (!is_melds_shuntu(tiles_melds)) {
        return false;
    }

    /* 両面待ちかどうか */
    // win_tileをmeldsから探して, 
    return true;
}
