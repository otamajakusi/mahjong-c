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

#include "score.h"

// 1翻
int is_pinfu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_tanyao(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_iipeiko(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_haku(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_hatsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_chun(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_ton(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_nan(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_sha(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_pei(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);

// 2翻
int is_toitoi(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_sanankou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_sanshoku_douko(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_sankantsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_shosangen(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_honroto(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_double_ton(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_double_nan(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_double_sha(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_double_pei(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);

// 以下食い下がり1翻
int is_sanshoku(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_ittsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_chanta(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);
int is_chiitoitsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);

// 3翻
int is_ryanpeiko(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg); // 二盃口, 門前, 一盃口の上位役
// 以下食い下がり2翻
int is_honitsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);   // 混一色, チンイツの下位役
int is_junchan(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);   // 純全帯么九(純チャン)(字牌を含まないチャンタ)

// 6翻
int is_chinitsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);  // 清一色(チンイツ)(字牌を使わないホンイツ)

// 役満
int is_kokushi(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);   // 国士無双, 門前
int is_suuankou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);  // 四暗刻, 門前
int is_daisangen(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg); // 大三元
int is_ryuisou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);   // 緑一色(緑發が入っていなくてもよい)
int is_tsuisou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);   // 字一色
int is_shosuushi(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg); // 小四喜
int is_daisuushi(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg); // 大四喜, 小四喜の上位役
int is_chinroto(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);  // 清老頭(すべて1,9牌のみで揃える,鳴きOK), ホンロウトウの上位役
int is_suukantsu(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg); // 四槓子
int is_chuuren_poutou(const Melds *tiles_melds, const Melds *open_melds, uint8_t head_tile_id, const ScoreConfig *cfg);  // 九蓮宝燈, 門前


#if 0
// 30符6翻
// https://npm2001.com/about/regulations/
// 日本プロ麻雀協会
// -> 7700
// https://www.ma-jan.or.jp/guide/game_rule.html
// 日本プロ麻雀連盟公式ルール
// -> 7700
// WRCルール
// -> 8000(切り上げ)
// https://m-league.jp/about/
// Mリーグ
// -> 8000(切り上げ)

// 連風牌の対子は2符 (https://npm2001.com/about/regulations/)
//
// 1 Han
// 1翻
pinfu;    // 平和, 門前
tanyao    // 断幺九
iipeiko   // 一盃口, 門前
haku      // 白
hatsu     // 發
chun      // 中
ton       // 東
nan       // 南
sha       // 西
pei       // 北

// 2翻
toitoi    // 対々和
sanankou  // 三暗刻
sanshoku_douko  // 三色同刻
sankantsu // 三槓子
shosangen // 小三元, 大三元の下位役
honroto   // 混老頭(ホンロウトウ)(すべて1,9,字牌), チャンタの上位役
// 以下食い下がり1翻
sanshoku  // 三色同順
ittsu     // 一気通貫
chanta    // 全帯么九(チャンタ)(1,9,字牌を含む構成. 混老頭と異なり123などOK. 混老頭と複合しない)
chiitoitsu  // 七対子(25符)

// 3翻
ryanpeiko // 二盃口, 門前, 一盃口の上位役
// 以下食い下がり2翻
honitsu   // 混一色, チンイツの下位役
junchan   // 純全帯么九(純チャン)(字牌を含まないチャンタ)

// 6翻
chinitsu  // 清一色(チンイツ)(字牌を使わないホンイツ)

// 役満
kokushi   // 国士無双, 門前
suuankou  // 四暗刻, 門前
daisangen // 大三元
ryuisou   // 緑一色(緑發が入っていなくてもよい)
tsuisou   // 字一色
shosuushi // 小四喜
daisuushi // 大四喜, 小四喜の上位役
chinroto  // 清老頭(すべて1,9牌のみで揃える,鳴きOK), ホンロウトウの上位役
suukantsu // 四槓子
chuuren_poutou  // 九蓮宝燈, 門前

// 役満ノート
大車輪は役満にならない
大四喜はダブル役満にならない
国士無双13面待ち(純正国士無双)はダブル役満にならない
四暗刻単騎待ちはダブル役満にならない
純正九蓮宝燈はダブル役満にならない

// Mリーグでのアガリメモ
// https://top3776.hatenablog.com/entry/2022/05/28/142033
#endif
