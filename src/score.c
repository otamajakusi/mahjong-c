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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "element.h"
#include "fu.h"
#include "yaku.h"

/*
 * 複数アガリの点数の比較
 * 同じアガリ配で複数通りのアガリがある場合に翻と符を必ず比較しないといけないか？
 * たとえば複数のアガリ A. 60符1翻, B. 30符2翻 があり得るか？
 * 場合:
 * 40符1翻と20符2翻(子1300)
 * 50符1翻と25符2翻(子1600)
 * 60符1翻と30符2翻(子2000)
 * 80符1翻と40符2翻(子2600)
 * 40符2翻と20符3翻(子2600)
 * 50符2翻と25符3翻(子3200)
 * 60符2翻と30符3翻(子3900)
 * 25符は七対子の場合で七対子が面子役になるためには二盃口となり50符1翻にならない.
 * 同じアガリ配で大きく異なる符となるためには順子構成が刻子構成にもなる場合で
 * 例えば 123,123,123,678,99 と 111,222,333,678,99 の場合
 * 20符2翻(平和,**自摸**)なので後者は常に三暗刻が付き1翻で止まらない.
 * 30符2翻と60符1翻は30の符の差があり, これは順子<->刻子の構成だけでは作れない.
 * 123,123,123 を 111,222,333 と捉えた場合16符の差しかない.
 * つまり翻を比較して同じなら符を比較して大小を比較できる.
 */

static void append_score(MJScore *score, uint32_t han, const char *yaku_name) {
  uint32_t name_len = (uint32_t)strlen(score->yaku_name);
  uint32_t n = MJ_MAX_YAKU_NAME_LEN - name_len - 1;
  assert(n >= strlen(yaku_name));
  strncat(&score->yaku_name[name_len], yaku_name, n);
  score->han += han;
}

static void append_score_kuisagari(MJScore *score, uint32_t han, const char *yaku_name, const Elements *melded) {
  if (!is_elements_concealed(melded)) {
    han--;
  }
  return append_score(score, han, yaku_name);
}

static void finalize_score(MJScore *score, uint32_t fu) { score->fu = fu; }

bool calc_score_with_tiles(MJScore *score, const Tiles *tiles, const ScoreConfig *cfg) {
  memset(score, 0, sizeof(MJScore));

  // 国士無双
  bool kokushi = is_kokushi(tiles);  // 13han
  if (kokushi) {
    append_score(score, 13, "kokushi ");
  }

  // 字一色
  bool tsuisou = is_tsuisou7(tiles);
  if (tsuisou) {
    append_score(score, 13, "tsuisou ");
  }

  if (kokushi || tsuisou) {
    finalize_score(score, kokushi ? 0 : FU_CHIITOITSU);
    return true;
  }

  // 七対子
  bool chiitoitsu = is_chiitoitsu(tiles);  // 2han
  if (chiitoitsu) {
    append_score(score, 2, "chiitoitsu ");
  } else {
    return false;
  }

  // 清一色(七対子)
  bool chinitsu = is_chinitsu7(tiles);  // 6han
  if (chinitsu) {
    append_score(score, 6, "chinitsu ");
  }

  if (!chinitsu) {  // honitsu and chinitsu are exclusive
    // 混一色(七対子)
    bool honitsu = is_honitsu7(tiles);  // 3han
    if (honitsu) {
      append_score(score, 3, "honitsu ");
    }
  }

  // 混老頭(七対子)
  bool honroto = is_honroto7(tiles);  // 2han
  if (honroto) {
    append_score(score, 2, "honroto ");
  }

  // 断么九(七対子)
  bool tanyao = is_tanyao7(tiles);  // 1han
  if (tanyao) {
    append_score(score, 1, "tanyao ");
  }

  if (!cfg->ron) {
    append_score(score, 1, "tsumo ");
  }
  finalize_score(score, FU_CHIITOITSU);
  return true;
}

bool calc_score(MJScore *score, const Elements *concealed, const Elements *melded, MJTileId pair,
                const ScoreConfig *cfg) {
  memset(score, 0, sizeof(MJScore));

  /*** 役満 ***/
  /* 四暗刻: 門前: 必要, 説明: 面子を暗刻(暗槓含む)で構成. 注意: ロンアガリで面子が揃う場合は明刻扱い. */
  bool suuankou = is_suuankou(concealed, melded, pair, cfg);
  if (suuankou) {
    append_score(score, 13, "suuankou ");
  }
  /* 大三元: 門前: 不要, 説明: 三元牌をすべて刻子で構成 */
  bool daisangen = is_daisangen(concealed, melded, pair, cfg);
  if (daisangen) {
    append_score(score, 13, "daisangen ");
  }
  /* 緑一色: 門前: 不要, 説明: 索子の23468と發のいずれかで構成. 發が含まれていなくてもよい */
  bool ryuisou = is_ryuisou(concealed, melded, pair, cfg);
  if (ryuisou) {
    append_score(score, 13, "ryuisou ");
  }
  /* 字一色: 門前: 不要, 配がすべて字牌で4面子1雀頭もしくは七対子 */
  bool tsuisou = is_tsuisou(concealed, melded, pair, cfg);
  if (tsuisou) {
    append_score(score, 13, "tsuisou ");
  }
  /* 小四喜: 門前: 不要, 1つの風牌の刻子と風牌の雀頭で構成 */
  bool shosuushi = is_shosuushi(concealed, melded, pair, cfg);
  if (shosuushi) {
    append_score(score, 13, "shosuushi ");
  }
  /* 大四喜: 門前: 不要, 風牌ですべての面子を構成 */
  bool daisuushi = is_daisuushi(concealed, melded, pair, cfg);
  if (daisuushi) {
    append_score(score, 13, "daisuushi ");
  }
  /* 清老頭: 門前: 不要, すべて老頭牌(1,9牌)で構成. 混老頭の上位役 (1,9牌は3種類しか無いので七対子と複合しない) */
  bool chinroto = is_chinroto(concealed, melded, pair, cfg);
  if (chinroto) {
    append_score(score, 13, "chinroto ");
  }
  /* 四槓子: 門前: 不要, 4面子を槓子で構成 */
  bool suukantsu = is_suukantsu(concealed, melded, pair, cfg);
  if (suukantsu) {
    append_score(score, 13, "suukantsu ");
  }
  /* 九蓮宝燈: 門前: 必要, 同種の数牌が1112345678999 + xで構成 */
  bool chuuren_poutou = is_chuuren_poutou(concealed, melded, pair, cfg);
  if (chuuren_poutou) {
    append_score(score, 13, "chuuren_poutou ");
  }
  if (suuankou || daisangen || ryuisou || tsuisou || shosuushi || daisuushi || chinroto || suukantsu ||
      chuuren_poutou) {
    finalize_score(score, calc_fu(concealed, melded, pair, cfg, false));
    return true;  // early return since yakuman
  }

  /*** 6翻(食い下がり5翻) ***/
  /* 清一色: 門前: 不要, 食い下がり: 5翻, 説明: 同種の数牌のみで構成. 七対子と複合する. */
  bool chinitsu = is_chinitsu(concealed, melded, pair, cfg);
  if (chinitsu) {
    append_score_kuisagari(score, 6, "chinitsu ", melded);
  }

  /*** 3翻 ***/
  /* 二盃口: 門前: 必須, 説明: 一盃口を2組を構成. 同種同順が2組でも成立. */
  bool ryanpeiko = is_ryanpeiko(concealed, melded, pair, cfg);
  if (ryanpeiko) {
    append_score(score, 3, "ryanpeiko ");
  }
  /*** 3翻(食い下がり2翻) ***/
  /* 混一色: 門前: 不要, 食い下がり: 2翻, 説明: 同種の数牌と字牌のみで構成. 七対子と複合する. */
  if (!chinitsu) {  // honitsu and chinitsu are exclusive
    bool honitsu = is_honitsu(concealed, melded, pair, cfg);
    if (honitsu) {
      append_score_kuisagari(score, 3, "honitsu ", melded);
    }
  }
  /* 純全帯么九: 門前: 不要, 食い下がり: 2翻, 説明: すべての面子と雀頭を老頭牌(1,9牌)を含む(123はOK). 混全帯么九に字牌が含まれない場合の構成.
     *             七対子と複合しない(複合する場合清老頭となるため) */
  bool junchan = is_junchan(concealed, melded, pair, cfg);
  if (junchan) {
    append_score_kuisagari(score, 3, "junchan ", melded);
  }

  /*** 2翻 ***/
  /* 対々和: 門前: 不要, 説明: 面子を刻子のみで構成 */
  bool toitoi = is_toitoi(concealed, melded, pair, cfg);
  if (toitoi) {
    append_score(score, 2, "toitoi ");
  }
  /* 三暗刻: 門前: 不要, 説明: 暗刻を3つ構成 */
  bool sanankou = is_sanankou(concealed, melded, pair, cfg);
  if (sanankou) {
    append_score(score, 2, "sanankou ");
  }
  /* 三色同刻: 門前: 不要, 説明: 同数異種の刻子を3つ構成 */
  bool sanshoku_douko = is_sanshoku_douko(concealed, melded, pair, cfg);
  if (sanshoku_douko) {
    append_score(score, 2, "sanshoku_douko ");
  }
  /* 三槓子: 門前: 不要, 説明: 槓子を3つ構成 */
  bool sankantsu = is_sankantsu(concealed, melded, pair, cfg);
  if (sankantsu) {
    append_score(score, 2, "sankantsu ");
  }
  /* 小三元: 門前: 不要, 説明: 三元牌を2つ刻子, 1つ雀頭で構成 */
  bool shosangen = is_shosangen(concealed, melded, pair, cfg);
  if (shosangen) {
    append_score(score, 2, "shosangen ");
  }
  /* 混老頭: 門前: 不要, 説明: 么九牌(1,9, 字牌)だけで構成. 七対子もしくは対々和と必ず複合する. */
  bool honroto = is_honroto(concealed, melded, pair, cfg);
  if (honroto) {
    append_score(score, 2, "honroto ");
  }
  /* ダブ東: 門前: 不要, 説明: 東が自風かつ場風のとき東の刻子を構成 */
  bool double_ton = is_double_ton(concealed, melded, pair, cfg);
  if (double_ton) {
    append_score(score, 2, "double_ton ");
  }
  /* ダブ南: 門前: 不要, 説明: 南が自風かつ場風のとき南の刻子を構成 */
  bool double_nan = is_double_nan(concealed, melded, pair, cfg);
  if (double_nan) {
    append_score(score, 2, "double_nan ");
  }
  /* ダブ西: 門前: 不要, 説明: 西が自風かつ場風のとき西の刻子を構成 */
  bool double_sha = is_double_sha(concealed, melded, pair, cfg);
  if (double_sha) {
    append_score(score, 2, "double_sha ");
  }
  /* ダブ北: 門前: 不要, 説明: 北が自風かつ場風のとき北の刻子を構成 */
  bool double_pei = is_double_pei(concealed, melded, pair, cfg);
  if (double_pei) {
    append_score(score, 2, "double_pei ");
  }

  /*** 2翻(食い下がり1翻) ***/
  /* 三色同順: 門前: 不要, 食い下がり: 1翻, 説明: 同数異種の順子を3つ構成 */
  bool sanshoku = is_sanshoku(concealed, melded, pair, cfg);
  if (sanshoku) {
    append_score_kuisagari(score, 2, "sanshoku ", melded);
  }
  /* 一気通貫: 門前: 不要, 食い下がり: 1翻, 説明: 同数順子で123,456,789を構成 */
  bool ittsu = is_ittsu(concealed, melded, pair, cfg);
  if (ittsu) {
    append_score_kuisagari(score, 2, "ittsu ", melded);
  }
  /* 混全帯么九: 門前: 不要, 食い下がり: 1翻, 説明: すべての面子と雀頭に么九牌(1,9,字牌)を含む(123はOK).
     *             七対子と複合しない(複合する場合混老頭となるため) */
  if (!junchan && !honroto) {  // (chanta and junchan) and (chanta and honroto) are exclusive respectivelly
    bool chanta = is_chanta(concealed, melded, pair, cfg);
    if (chanta) {
      append_score_kuisagari(score, 2, "chanta ", melded);
    }
  }

  /* 平和: 門前: 必須, 説明: 役牌以外で構成, 面子を順子のみで構成し両面待ちで上がる. ロンで30符, ツモで20符 */
  bool pinfu = is_pinfu(concealed, melded, pair, cfg);
  if (pinfu) {
    append_score(score, 1, "pinfu ");
  }
  /* 断么九: 門前: 不要, 説明: 么九牌以外で構成 */
  bool tanyao = is_tanyao(concealed, melded, pair, cfg);
  if (tanyao) {
    append_score(score, 1, "tanyao ");
  }

  /* 一盃口: 門前: 必須, 説明: 同数同種の数牌の順子を2組を構成 */
  if (!ryanpeiko) {  // ryanpeiko and iipeiko are exclusive
    bool iipeiko = is_iipeiko(concealed, melded, pair, cfg);
    if (iipeiko) {
      append_score(score, 1, "iipeiko ");
    }
  }

  /* 白: 門前: 不要, 説明: 白の刻子を構成 */
  bool haku = is_haku(concealed, melded, pair, cfg);
  if (haku) {
    append_score(score, 1, "haku ");
  }
  /* 發: 門前: 不要, 説明: 發の刻子を構成 */
  bool hatsu = is_hatsu(concealed, melded, pair, cfg);
  if (hatsu) {
    append_score(score, 1, "hatsu ");
  }
  /* 中: 門前: 不要, 説明: 中の刻子を構成 */
  bool chun = is_chun(concealed, melded, pair, cfg);
  if (chun) {
    append_score(score, 1, "chun ");
  }
  /* 東: 門前: 不要, 説明: 東が役牌のとき東の刻子を構成 */
  if (!double_ton) {
    bool ton = is_ton(concealed, melded, pair, cfg);
    if (ton) {
      append_score(score, 1, "ton ");
    }
  }
  /* 南: 門前: 不要, 説明: 南が役牌のとき南の刻子を構成 */
  if (!double_nan) {
    bool nan = is_nan(concealed, melded, pair, cfg);
    if (nan) {
      append_score(score, 1, "nan ");
    }
  }
  /* 西: 門前: 不要, 説明: 西が役牌のとき西の刻子を構成 */
  if (!double_sha) {
    bool sha = is_sha(concealed, melded, pair, cfg);
    if (sha) {
      append_score(score, 1, "sha ");
    }
  }
  /* 北: 門前: 不要, 説明: 北が役牌のとき北の刻子を構成 */
  if (!double_pei) {
    bool pei = is_pei(concealed, melded, pair, cfg);
    if (pei) {
      append_score(score, 1, "pei ");
    }
  }
  /* 門前清自摸和: 門前: 必要 */
  bool tsumo = is_tsumo(concealed, melded, pair, cfg);
  if (tsumo) {
    append_score(score, 1, "tsumo ");
  }
  finalize_score(score, calc_fu(concealed, melded, pair, cfg, pinfu));
  return true;
}
