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

#include "element.h"
#include "mahjong.h"
#include "score.h"

/*
 * 基本符: 20符
 * [ロン/ツモ]
 * 門前ロン: +10符
 * ツモ: +2符(ピンフの場合0符)
 * (例外: ツモ+ピンフは20符, チートイツは(ツモ/ロンに限らず)25符 で計算)
 * [雀頭]
 * 役牌(自風牌, 場風牌, 三元牌): +2符
 * [待ち]
 * カンチャン待ち: +2符
 * ペンチャン待ち: +2符
 * 単騎待ち: +2符 TODO: http://www.interq.or.jp/silver/s883/tensuukeisan.html の待ちのテストを実施する
 * [面子]
 * 明刻: +2(中張牌), +4(么九牌牌)
 * 暗刻: +4(中張牌), +8(么九牌牌)
 * 明槓: +8(中張牌), +16(么九牌牌)
 * 暗槓: +16(中張牌), +32(么九牌牌)
 *
 * ロンアガリにより刻子が作成された場合、その刻子は明刻、ツモアガリにより作成された場合、その刻子は暗刻として扱う.
 * 喰いタンは20符の場合30符に切り上げる.
 * 20符1翻は存在しない. 20符はツモ・ピンフのみで翻数は2翻.
 *
 * 嶺上牌はツモ符+2がつく https://www.ma-jan.or.jp/guide/game_rule.html
 * Mリーグも同じ
 *
 * このサイトの図がわかりやすい
 * https://majandofu.com/fu-calculation
 * このサイトに高点法の例がいくつかあるのでテストに使う
 * https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q1285091887
 */

#define FU_CHIITOITSU 25
uint32_t calc_fu(const Elements *concealed, const Elements *melded, MJTileId pair, const ScoreConfig *cfg, bool pinfu);
