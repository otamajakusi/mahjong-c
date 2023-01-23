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

#include "fu.h"
#include "tile.h"

static uint32_t round_up(uint32_t fu, uint32_t n) {
    return ((fu + (n - 1)) / n) * n;
}

uint32_t calc_fu(const Elements *concealed, const Elements *melded, MJTileId pair, const ScoreConfig *cfg, bool pinfu) {

    if (pinfu) {
        if (cfg->ron) {
            return 30;
        } else {
            return 20; // 自摸, 平和
        }
    }

    uint32_t fu = 20;
    if (!cfg->ron) { // 自摸
        fu += 2;
    }
    if (is_elements_concealed(melded) && cfg->ron) { // 門前ロン(暗槓は門前扱い)
        fu += 10;
    }
    if (is_tile_id_dragon(pair)) {
        fu += 2;
    }
    if (cfg->player_wind == pair || cfg->round_wind == pair) { // 連風牌は2符
        fu += 2;
    }
    if (is_kanchan_machi(concealed, cfg->win_tile) || 
        is_penchan_machi(concealed, cfg->win_tile) ||
        is_tanki_machi(pair, cfg->win_tile)) {
        fu += 2;
    }
    // concealed
    for (uint32_t i = 0; i < concealed->len; i ++) {
        const Element *elem = &concealed->meld[i];
        if (is_element_triplets(elem)) {
            MJTileId tile_id = elem->tile_id[0];
            if (is_element_chunchan(elem)) {
                if (tile_id == cfg->win_tile && cfg->ron) { // この刻子はロンで作成された刻子なので明刻扱い
                    fu += 2; // 明刻(中張牌)
                } else {
                    fu += 4; // 暗刻(中張牌)
                }
            } else {
                if (tile_id == cfg->win_tile && cfg->ron) { // この刻子はロンで作成された刻子なので明刻扱い
                    fu += 4; // 明刻(么九牌)
                } else {
                    fu += 8; // 暗刻(么九牌)
                }
            }
        }
    }
    // melded
    for (uint32_t i = 0; i < melded->len; i ++) {
        const Element *elem = &melded->meld[i];
        if (is_element_triplets(elem)) {
            if (is_element_chunchan(elem)) {
                fu += 2; // 明刻(中張牌)
            } else {
                fu += 4; // 明刻(么九牌)
            }
        } else if (is_element_fours(elem)) {
            if (!is_element_concealed(elem)) {
                if (is_element_chunchan(elem)) {
                    fu += 8; // 明槓(中張牌)
                } else {
                    fu += 16; // 明槓(么九牌)
                }
            } else {
                if (is_element_chunchan(elem)) {
                    fu += 16; // 暗槓(中張牌)
                } else {
                    fu += 32; // 暗槓(么九牌)
                }
            }
        }
    }
    fu = fu == 20 ? 30 : fu; // 20符は30符に切り上げる
    return round_up(fu, 10); // 1の位を切り上げ
}

