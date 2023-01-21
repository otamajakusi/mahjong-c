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

#include "score.h"
#include "element.h"
#include "yaku.h"

bool calc_score_concealed_hands(MJScore *score, const Tiles *tiles, const ScoreConfig *cfg) {
    (void)cfg;
    memset(score, 0, sizeof(MJScore));
    bool kokushi = is_kokushi(tiles); // 13han
    if (kokushi) {
        score->han = 13;
        score->fu = 0;
        strcpy(score->yaku_name, "kokushi,");
        return true;
    }
    bool chiitoitsu = is_chiitoitsu(tiles); // 2han
    if (!chiitoitsu) {
        return false;
    }
    // 断么九, 混老頭, 混一色, 清一色, 字一色, 
    bool tanyao = is_tanyao7(tiles); // 1han
    if (tanyao) {
        score->han = 2+1;
        score->fu = 25;
        strcpy(score->yaku_name, "chiitoitsu,tanyao,");
        return true;
    }
    bool honroto = is_honroto7(tiles); // 2han
    if (honroto) {
        score->han = 2+2;
        score->fu = 25;
        strcpy(score->yaku_name, "chiitoitsu,honroto,");
        return true;
    }
    bool honitsu = is_honitsu7(tiles); // 3han
    bool chinitsu = is_chinitsu7(tiles); // 6han
    if (chinitsu) {
        score->han = 2+6;
        score->fu = 25;
        strcpy(score->yaku_name, "chiitoitsu,chinitsu,");
        return true;
    } else if (honitsu) {
        score->han = 2+3;
        score->fu = 25;
        strcpy(score->yaku_name, "chiitoitsu,honitsu,");
        return true;
    }
    score->han = 2;
    score->fu = 25;
    strcpy(score->yaku_name, "chiitoitsu,");
    return true;
}

bool calc_score(MJScore *score, const Elements *concealed, const Elements *melded, MJTileId pair, const ScoreConfig *cfg) {
    (void)score;
    (void)concealed;
    (void)melded;
    (void)pair;
    (void)cfg;
    return 0;
}
