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

#include "mahjong.h"
#include "element.h"
#include "score.h"

/*** 1翻 ***/
/* 平和: 門前: 必須, 説明: 役牌以外で構成, 面子を順子のみで構成し両面待ちで上がる. ロンで30符, ツモで20符 */
int is_pinfu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 断么九: 門前: 不要, 説明: 么九牌以外で構成 */
int is_tanyao(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 一盃口: 門前: 必須, 説明: 同数同種の数牌の順子を2組を構成 */
int is_iipeiko(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 白: 門前: 不要, 説明: 白の刻子を構成 */
int is_haku(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 發: 門前: 不要, 説明: 發の刻子を構成 */
int is_hatsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 中: 門前: 不要, 説明: 中の刻子を構成 */
int is_chun(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 東: 門前: 不要, 説明: 東が役牌のとき東の刻子を構成 */
int is_ton(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 南: 門前: 不要, 説明: 南が役牌のとき南の刻子を構成 */
int is_nan(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 西: 門前: 不要, 説明: 西が役牌のとき西の刻子を構成 */
int is_sha(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 北: 門前: 不要, 説明: 北が役牌のとき北の刻子を構成 */
int is_pei(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);

/*** 2翻 ***/
/* 対々和: 門前: 不要, 説明: 面子を刻子のみで構成 */
int is_toitoi(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 三暗刻: 門前: 不要, 説明: 暗刻を3つ構成 */
int is_sanankou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 三色同刻: 門前: 不要, 説明: 同数異種の刻子を3つ構成 */
int is_sanshoku_douko(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 三槓子: 門前: 不要, 説明: 槓子を3つ構成 */
int is_sankantsu(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 小三元: 門前: 不要, 説明: 三元牌を2つ刻子, 1つ雀頭で構成 */
int is_shosangen(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
/* 混老頭: 門前: 不要, 説明: 么九牌(1,9, 字牌)だけで構成(七対子もしくは対々和と必ず複合する) */
int is_honroto(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);
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
int is_chuuren_poutou(const Elements *concealed_elems, const Elements *melded_elems, MJTileId pair_tile, const ScoreConfig *cfg);

/*
 * [30符6翻の扱い]
 * 1. 日本プロ麻雀協会
 *    https://npm2001.com/about/regulations/
 *    7700点
 * 2. 日本プロ麻雀連盟公式ルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    7700点
 * 3. 日本プロ麻雀連盟WRCルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    8000点(切り上げ)
 * 4. Mリーグ
 *    https://m-league.jp/about/
 *    8000点(切り上げ)
 *
 * [連風牌の対子の符]
 * 1. 日本プロ麻雀協会
 *    https://npm2001.com/about/regulations/
 *    2符
 * 2. 日本プロ麻雀連盟公式ルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    4符
 * 3. 日本プロ麻雀連盟WRCルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    2符
 * 4. Mリーグ
 *    https://m-league.jp/about/
 *    2符
 *
 * [嶺上牌のツモ符]
 * 1. 日本プロ麻雀協会
 *    https://npm2001.com/about/regulations/
 *    2符(明記が無いが以下の記述あり「嶺上牌によるアガリは全てツモアガリとする」)
 * 2. 日本プロ麻雀連盟公式ルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    0符(「嶺上牌にツモ符はつかない」)
 * 3. 日本プロ麻雀連盟WRCルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    2符(「嶺上牌にツモ符が２符つく」)
 * 4. Mリーグ
 *    https://m-league.jp/about/
 *    2符(明記が無いが以下の記述あり「ツモアガリとする」)
 *
 * [役満の種類]
 * 1. 日本プロ麻雀協会
 *    https://npm2001.com/about/regulations/
 *    天和, 地和, 十三幺九（国士無双）, 四暗刻, 大三元, 緑一色, 字一色, 小四喜, 大四喜, 清老頭, 四槓子, 九蓮宝燈
 * 2. 日本プロ麻雀連盟公式ルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    明記なし
 * 3. 日本プロ麻雀連盟WRCルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    明記なし
 * 4. Mリーグ
 *    https://m-league.jp/about/
 *    天和, 地和, 国士無双, 四暗刻, 大三元, 緑一色, 字一色, 小四喜, 大四喜, 清老頭, 四槓子, 九蓮宝燈
 *
 * [緑一色の發]
 * 1. 日本プロ麻雀協会
 *    https://npm2001.com/about/regulations/
 *    發が必要「索子牌の2・3・4・6・8、および緑發で構成されたアガリ形」
 * 2. 日本プロ麻雀連盟公式ルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    發が必要(「緑一色は發が含まれなければいけない」
 * 3. 日本プロ麻雀連盟WRCルール
 *    https://www.ma-jan.or.jp/guide/game_rule.html
 *    發はなくてもよい(「緑一色は發がなくても可」)
 * 4. Mリーグ
 *    https://m-league.jp/about/
 *    發はなくてもよい(「緑發が入っていなくてもよい」)
 */
