#include "test_score.h"

#include <assert.h>
#include <stdio.h>

#include "test_util.h"

static void _test_calc_score_with_tiles(
    /* input */
    MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6, MJTileId t7, MJTileId t8, MJTileId t9,
    MJTileId t10, MJTileId t11, MJTileId t12, MJTileId pair, MJTileId win_tile, bool ron, MJTileId player_wind,
    MJTileId round_wind,
    /* expected */
    bool retval, uint32_t han, uint32_t fu, const char *yaku_name) {
  MJBaseScore score;
  memset(&score, -1, sizeof(MJBaseScore));
  Tiles tiles;
  MJHands hands = {
      {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, pair, pair},
      3 * 4 + 2,
  };
  assert(gen_tiles_from_hands(&tiles, &hands));

  ScoreConfig cfg = {win_tile, ron, player_wind, round_wind};
  assert(calc_score_with_tiles(&score, &tiles, &cfg) == retval);
  if (retval) {
    assert(score.han == han);
    assert(score.fu == fu);
    assert(strcmp(score.yaku_name, yaku_name) == 0);
  }
}

void test_calc_score_with_tiles() {
  /*                                1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,pa,win,ron,pw,rw, ret, han,fu, yaku_name */
  _test_calc_score_with_tiles(dw, dg, dr, m1, p1, s1, m9, p9, s9, wt, wn, ws, wp, dr, 1, wt, wt, true, 13, 0,
                              "kokushi ");
  _test_calc_score_with_tiles(m1, m1, m2, m2, m3, m3, p3, p3, p4, p4, p5, p5, wt, wt, 1, wt, wt, true, 2, 25,
                              "chiitoitsu ");
  _test_calc_score_with_tiles(m1, m1, m2, m2, m3, m3, p3, p3, p4, p4, p5, p5, wt, wt, 0, wt, wt, true, 3, 25,
                              "chiitoitsu tsumo ");
  _test_calc_score_with_tiles(s2, s2, m2, m2, m3, m3, p3, p3, p4, p4, p5, p5, p6, p5, 1, wt, wt, true, 3, 25,
                              "chiitoitsu tanyao ");
  _test_calc_score_with_tiles(wt, wt, wn, wn, ws, ws, wp, wp, dw, dw, dg, dg, dr, dr, 1, wt, wt, true, 13, 25,
                              "tsuisou ");
  _test_calc_score_with_tiles(p1, p1, p3, p3, p4, p4, p6, p6, wt, wt, wn, wn, wp, wp, 1, wt, wt, true, 5, 25,
                              "chiitoitsu honitsu ");
  _test_calc_score_with_tiles(p1, p1, p3, p3, p4, p4, p6, p6, p7, p7, p8, p8, p9, p1, 1, wt, wt, true, 8, 25,
                              "chiitoitsu chinitsu ");
  _test_calc_score_with_tiles(p1, p1, p9, p9, m1, m1, s1, s1, wt, wt, dw, dw, dr, dr, 1, wt, wt, true, 4, 25,
                              "chiitoitsu honroto ");
  _test_calc_score_with_tiles(p1, p1, p9, p9, m1, m1, s1, s1, wt, wt, dw, dw, dr, dr, 1, wt, wt, true, 4, 25,
                              "chiitoitsu honroto ");
  _test_calc_score_with_tiles(p1, p1, p9, p9, m1, m1, s1, s1, wt, wt, dw, dw, m1, dr, 1, wt, wt, false, 0, 0,
                              "");  // non chiitoitsu
}

static void _test_calc_score0(/* 門前 */
                              /* input */
                              MJTileId e11, MJTileId e12, MJTileId e13, MJTileId e21, MJTileId e22, MJTileId e23,
                              MJTileId e31, MJTileId e32, MJTileId e33, MJTileId e41, MJTileId e42, MJTileId e43,
                              MJTileId pair, MJTileId win_tile, bool ron, MJTileId player_wind, MJTileId round_wind,
                              /* expected */
                              uint32_t han, uint32_t fu, const char *yaku_name) {
  MJBaseScore score;
  memset(&score, -1, sizeof(MJBaseScore));
  Elements concealed;
  Elements melded;
  MJMelds melds = {
      {
          {{e11, e12, e13}, 3, true, xx},
          {{e21, e22, e23}, 3, true, xx},
          {{e31, e32, e33}, 3, true, xx},
          {{e41, e42, e43}, 3, true, xx},
      },
      4,
  };
  assert(gen_elements_from_melds(&concealed, &melds));
  memset(&melded, 0, sizeof(Elements));

  ScoreConfig cfg = {win_tile, ron, player_wind, round_wind};
  calc_score(&score, &concealed, &melded, pair, &cfg);
  fprintf(stderr, "yaku %s, han %d, fu %d\n", score.yaku_name, score.han, score.fu);
  assert(score.han == han);
  assert(score.fu == fu);
  assert(strcmp(score.yaku_name, yaku_name) == 0);
}

static void _test_calc_score1(/* 全副露(暗槓含む) */
                              /* input */
                              MJTileId e11, MJTileId e12, MJTileId e13,
                              MJTileId e14,  // if positive fours is created
                              bool e1_concealed, MJTileId e21, MJTileId e22, MJTileId e23,
                              MJTileId e24,  // if positive fours is created
                              bool e2_concealed, MJTileId e31, MJTileId e32, MJTileId e33,
                              MJTileId e34,  // if positive fours is created
                              bool e3_concealed, MJTileId e41, MJTileId e42, MJTileId e43,
                              MJTileId e44,  // if positive fours is created
                              bool e4_concealed, MJTileId pair, MJTileId win_tile, bool ron, MJTileId player_wind,
                              MJTileId round_wind,
                              /* expected */
                              uint32_t han, uint32_t fu, const char *yaku_name) {
  MJBaseScore score;
  memset(&score, -1, sizeof(MJBaseScore));
  Elements concealed;
  Elements melded;
  MJMelds melds = {
      {
          {{
               e11,
               e12,
               e13,
               e14,
           },
           (int)e14 < 0 ? 3 : 4,
           e1_concealed,
           xx},
          {{
               e21,
               e22,
               e23,
               e24,
           },
           (int)e24 < 0 ? 3 : 4,
           e2_concealed,
           xx},
          {{
               e31,
               e32,
               e33,
               e34,
           },
           (int)e34 < 0 ? 3 : 4,
           e3_concealed,
           xx},
          {{
               e41,
               e42,
               e43,
               e44,
           },
           (int)e44 < 0 ? 3 : 4,
           e4_concealed,
           xx},
      },
      4,
  };
  assert(gen_elements_from_melds(&melded, &melds));
  memset(&concealed, 0, sizeof(Elements));

  ScoreConfig cfg = {win_tile, ron, player_wind, round_wind};
  calc_score(&score, &concealed, &melded, pair, &cfg);
  fprintf(stderr, "yaku %s, han %d, fu %d\n", score.yaku_name, score.han, score.fu);
  assert(score.han == han);
  assert(score.fu == fu);
  assert(strcmp(score.yaku_name, yaku_name) == 0);
}

void test_calc_score() {
  /*                                                       pair,win,ron, pw, rw,han, fu, yaku_name */
  /*** 1翻 ***/
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, s7, s8, s9, m1, s9, 1, wt, wt, 1, 30, "pinfu ");  // 平和(ロン)
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, s7, s8, s9, m1, s9, 0, wt, wt, 2, 20,
                    "pinfu tsumo ");  // 平和(自摸)
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, s7, s8, s9, m1, m3, 1, wt, wt, 0, 40,
                    "");  // 役なし(ペンチャン待ち)
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, s7, s8, s9, m1, p3, 1, wt, wt, 0, 40,
                    "");  // 役なし(カンチャン待ち)
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, s7, s8, s9, m1, m3, 0, wt, wt, 1, 30, "tsumo ");   // 自摸
  _test_calc_score0(m2, m3, m4, p2, p3, p4, s3, s4, s5, s6, s7, s8, m2, p3, 1, wt, wt, 1, 40, "tanyao ");  // 断么九
  _test_calc_score0(m2, m3, m4, p2, p3, p4, p2, p3, p4, s6, s7, s8, m2, p4, 1, wt, wt, 3, 30,
                    "pinfu tanyao iipeiko ");  // 平和,断么九,一盃口
  _test_calc_score0(m1, m1, m1, p2, p3, p4, p2, p3, p4, s6, s7, s8, m2, p4, 1, wt, wt, 1, 40, "iipeiko ");  // 一盃口
  _test_calc_score0(m1, m1, m1, p2, p3, p4, p2, p3, p4, dw, dw, dw, dr, p4, 1, wt, wt, 2, 50,
                    "iipeiko haku ");  // 一盃口,白
  _test_calc_score0(m1, m2, m3, p2, p3, p4, dg, dg, dg, dw, dw, dw, m9, p3, 2, wt, wt, 2, 50, "haku hatsu ");  // 白,發
  _test_calc_score0(m1, m2, m3, dr, dr, dr, p3, p3, p3, dw, dw, dw, m9, p3, 2, wt, wt, 2, 50, "haku chun ");  // 白,中
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, wt, wt, wt, m1, wt, 1, wt, wn, 1, 40, "ton ");        // 東
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, wn, wn, wn, m1, wn, 1, wt, wn, 1, 40, "nan ");        // 南
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, ws, ws, ws, m1, ws, 1, ws, wn, 1, 40, "sha ");        // 西
  _test_calc_score0(m1, m2, m3, p2, p3, p4, s3, s4, s5, wp, wp, wp, m1, wp, 1, ws, wp, 1, 40, "pei ");        // 北
  /*** 2翻(食い下がり1翻) ***/
  _test_calc_score0(m2, m3, m4, p2, p3, p4, s2, s3, s4, wp, wp, wp, m1, wp, 1, ws, wt, 2, 40, "sanshoku ");  // 三色同順
  _test_calc_score0(m1, m2, m3, m7, m8, m9, m4, m5, m6, wp, wp, wp, s5, wp, 1, ws, wt, 2, 40, "ittsu ");  // 一気通貫
  _test_calc_score0(m1, m2, m3, p7, p8, p9, m4, m5, m6, wp, wp, wp, s5, wp, 1, ws, wt, 0, 40, "");  // 役なし(x一気通貫)
  _test_calc_score0(m1, m2, m3, p7, p8, p9, s1, s2, s3, wp, wp, wp, s1, wp, 1, ws, wt, 2, 40, "chanta ");  // 混全帯么九
  /*** 2翻 ***/
  _test_calc_score0(m1, m1, m1, p7, p7, p7, s2, s2, s2, s4, s4, s4, s3, p7, 1, ws, wt, 4, 50,
                    "toitoi sanankou ");  // 対々和,三暗刻
  _test_calc_score0(m1, m1, m1, p7, p8, p9, s2, s2, s2, s4, s4, s4, s3, p7, 1, ws, wt, 2, 50, "sanankou ");  // 三暗刻
  _test_calc_score0(m2, m2, m2, p7, p8, p9, s2, s2, s2, p2, p2, p2, s3, p2, 1, ws, wt, 2, 40,
                    "sanshoku_douko ");  // 三色同刻(p2が明刻扱いでちょうど40符)
                                         // (三槓子は_test_calc_score1でテストする)
  _test_calc_score0(dw, dw, dw, dr, dr, dr, s3, s4, s5, p2, p2, p2, dg, dr, 1, ws, wt, 4, 50,
                    "shosangen haku chun ");  // 小三元,白,中
  _test_calc_score0(m1, m1, m1, m9, m9, m9, s1, s1, s1, wp, wp, wp, dg, m1, 1, ws, wt, 6, 60,
                    "toitoi sanankou honroto ");  // 対々和,三暗刻,混老頭
  _test_calc_score0(m1, m1, m1, m6, m7, m8, s1, s1, s1, wt, wt, wt, m2, m1, 1, wt, wt, 2, 50, "double_ton ");  // ダブ東
  _test_calc_score0(m1, m1, m1, m6, m7, m8, s1, s1, s1, wn, wn, wn, m2, m1, 1, wn, wn, 2, 50, "double_nan ");  // ダブ南
  _test_calc_score0(m1, m1, m1, m6, m7, m8, s1, s1, s1, ws, ws, ws, m2, m1, 1, ws, ws, 2, 50, "double_sha ");  // ダブ西
  _test_calc_score0(m1, m1, m1, m6, m7, m8, s1, s1, s1, wp, wp, wp, m2, m1, 1, wp, wp, 2, 50, "double_pei ");  // ダブ北
  /*** 3翻(食い下がり2翻) ***/
  _test_calc_score0(m1, m1, m1, m6, m7, m8, m5, m6, m7, wp, wp, wp, m2, m1, 1, wt, wt, 3, 50, "honitsu ");  // 混一色
  _test_calc_score0(p5, p5, p5, p6, p7, p8, p2, p2, p2, ws, ws, ws, wp, ws, 1, ws, wt, 4, 50,
                    "honitsu sha ");  // 混一色(頭が字牌)
  _test_calc_score0(m1, m1, m1, m7, m8, m9, p1, p2, p3, p9, p9, p9, s9, m1, 1, wt, wt, 3, 50,
                    "junchan ");  // 純全帯么九
  /*** 3翻 ***/
  _test_calc_score0(m2, m3, m4, p7, p8, p9, m2, m3, m4, p7, p8, p9, m2, m3, 1, wt, wt, 3, 40, "ryanpeiko ");  // 二盃口
  _test_calc_score0(m2, m3, m4, p7, p8, p9, m2, m3, m4, p7, p8, p9, m2, m2, 1, wt, wt, 4, 30,
                    "ryanpeiko pinfu ");  // 二盃口,平和
  /*** 6翻(食い下がり5翻) ***/
  _test_calc_score0(m2, m3, m4, m3, m4, m5, m5, m5, m5, m7, m8, m9, m1, m9, 1, wt, wt, 6, 40, "chinitsu ");  // 清一色
  /*** 役満 ***/
  _test_calc_score0(m2, m2, m2, p3, p3, p3, m5, m5, m5, m7, m7, m7, m1, m7, 1, wt, wt, 4, 50,
                    "toitoi sanankou ");  // 対々和,三暗刻(x四暗刻)
  _test_calc_score0(m2, m2, m2, p3, p3, p3, m5, m5, m5, m7, m7, m7, m1, m1, 1, wt, wt, 13, 50, "suuankou ");  // 四暗刻
  _test_calc_score0(dw, dw, dw, dr, dr, dr, s3, s4, s5, dg, dg, dg, m1, s3, 1, ws, wt, 13, 60, "daisangen ");  // 大三元
  _test_calc_score0(s2, s2, s2, s2, s3, s4, s6, s6, s6, dg, dg, dg, s8, dg, 1, ws, wt, 13, 50, "ryuisou ");  // 緑一色
  _test_calc_score0(wt, wt, wt, dw, dw, dw, dg, dg, dg, wn, wn, wn, wp, wn, 1, ws, wt, 13, 60, "tsuisou ");  // 字一色
  _test_calc_score0(wt, wt, wt, ws, ws, ws, s7, s8, s9, wn, wn, wn, wp, wn, 1, ws, wt, 13, 50, "shosuushi ");  // 小四喜
  _test_calc_score0(wt, wt, wt, ws, ws, ws, wp, wp, wp, wn, wn, wn, m2, wn, 1, ws, wt, 13, 60, "daisuushi ");  // 大四喜
  _test_calc_score0(s9, s9, s9, s1, s1, s1, p1, p1, p1, p9, p9, p9, m1, p9, 1, ws, wt, 13, 60, "chinroto ");  // 清老頭
  // (四槓子は_test_calc_score1でテストする)
  _test_calc_score0(m1, m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, 1, ws, wt, 13, 40,
                    "chuuren_poutou ");  // 九蓮宝燈(x=1)
  _test_calc_score0(m1, m1, m1, m3, m4, m5, m6, m7, m8, m9, m9, m9, m2, m9, 1, ws, wt, 13, 50,
                    "chuuren_poutou ");  // 九蓮宝燈(x=2)
  _test_calc_score0(m1, m2, m3, m3, m4, m5, m6, m7, m8, m9, m9, m9, m1, m9, 1, ws, wt, 13, 40,
                    "chuuren_poutou ");  // 九蓮宝燈(x=3)
  _test_calc_score0(m1, m1, m1, m2, m3, m4, m4, m5, m6, m7, m8, m9, m9, m9, 1, ws, wt, 13, 40,
                    "chuuren_poutou ");  // 九蓮宝燈(x=4)
  _test_calc_score0(m1, m1, m1, m2, m3, m4, m6, m7, m8, m9, m9, m9, m5, m9, 1, ws, wt, 13, 50,
                    "chuuren_poutou ");  // 九蓮宝燈(x=5)
  _test_calc_score0(m1, m2, m3, m4, m5, m6, m6, m7, m8, m9, m9, m9, m1, m9, 1, ws, wt, 13, 40,
                    "chuuren_poutou ");  // 九蓮宝燈(x=6)
  _test_calc_score0(m1, m1, m1, m2, m3, m4, m5, m6, m7, m7, m8, m9, m9, m9, 1, ws, wt, 13, 40,
                    "chuuren_poutou ");  // 九蓮宝燈(x=7)
  _test_calc_score0(m1, m1, m1, m2, m3, m4, m5, m6, m7, m9, m9, m9, m8, m9, 1, ws, wt, 13, 50,
                    "chuuren_poutou ");  // 九蓮宝燈(x=8)
  _test_calc_score0(m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m9, m1, m9, 1, ws, wt, 13, 40,
                    "chuuren_poutou ");  // 九蓮宝燈(x=9)
  _test_calc_score0(wt, wt, wt, dw, dw, dw, dg, dg, dg, dr, dr, dr, wp, dr, 1, ws, wt, 26, 60,
                    "daisangen tsuisou ");  // 大三元,字一色
  _test_calc_score0(wt, wt, wt, dw, dw, dw, dg, dg, dg, dr, dr, dr, wp, wp, 1, ws, wt, 39, 70,
                    "suuankou daisangen tsuisou ");  // 四暗刻,大三元,字一色
  _test_calc_score0(wt, wt, wt, dw, dw, dw, dg, dg, dg, dr, dr, dr, wp, wp, 0, ws, wt, 39, 60,
                    "suuankou daisangen tsuisou ");  // 四暗刻,大三元,字一色
  /*                                                       pair,win,ron, pw, rw,han, fu, yaku_name */

  /*                 1, 2, 3, 4, c,  1, 2, 3, 4, c,  1, 2, 3, 4, c,  1, 2, 3, 4, c,pair,win,ron, pw, rw,han, fu, yaku_name */
  /*** 1翻 ***/
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, s7, s8, s9, xx, 0, m1, s9, 1, wt, wt, 0,
                    30,
                    "");  // 役なし(x平和(ロン))
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, s7, s8, s9, xx, 0, m1, s9, 0, wt, wt, 0,
                    30,
                    "");  // 役なし(x平和(自摸))
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, s7, s8, s9, xx, 0, m1, m3, 1, wt, wt, 0,
                    30,
                    "");  // 役なし(ペンチャン待ち)
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, s7, s8, s9, xx, 0, m1, p3, 1, wt, wt, 0,
                    30,
                    "");  // 役なし(カンチャン待ち)
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, s7, s8, s9, xx, 0, m1, p3, 0, wt, wt, 0,
                    30,
                    "");  // 役なし(x自摸)
  _test_calc_score1(m2, m3, m4, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, s6, s7, s8, xx, 0, m2, p3, 0, wt, wt, 1,
                    30,
                    "tanyao ");  // 断么九
  _test_calc_score1(m2, m3, m4, xx, 0, p2, p3, p4, xx, 0, p3, p4, p5, xx, 0, s6, s7, s8, xx, 0, m2, p3, 0, wt, wt, 1,
                    30,
                    "tanyao ");  // 断么九(x平和,x断么九)
  _test_calc_score1(m1, m1, m1, xx, 0, p2, p3, p4, xx, 0, p2, p3, p4, xx, 0, s6, s7, s8, xx, 0, m2, p4, 0, wt, wt, 0,
                    30,
                    "");  // 役なし(x一盃口)
  _test_calc_score1(m1, m1, m1, xx, 0, p2, p3, p4, xx, 0, p2, p3, p4, xx, 0, dw, dw, dw, xx, 0, m2, p4, 0, wt, wt, 1,
                    30,
                    "haku ");  // 白(x一盃口)
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, dg, dg, dg, xx, 0, dw, dw, dw, xx, 0, m9, p3, 0, wt, wt, 2,
                    30,
                    "haku hatsu ");  // 白,發
  _test_calc_score1(m1, m2, m3, xx, 0, dr, dr, dr, xx, 0, p3, p3, p3, xx, 0, dw, dw, dw, xx, 0, m9, p3, 0, wt, wt, 2,
                    40,
                    "haku chun ");  // 白,中
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, wt, wt, wt, xx, 0, m1, wt, 0, wt, wn, 1,
                    30, "ton ");  // 東
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, wn, wn, wn, xx, 0, m1, wn, 0, wt, wn, 1,
                    30, "nan ");  // 南
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, ws, ws, ws, xx, 0, m1, ws, 0, ws, wn, 1,
                    30, "sha ");  // 西
  _test_calc_score1(m1, m2, m3, xx, 0, p2, p3, p4, xx, 0, s3, s4, s5, xx, 0, wp, wp, wp, xx, 0, m1, wp, 0, ws, wp, 1,
                    30, "pei ");  // 北
  /*** 2翻(食い下がり1翻) ***/
  _test_calc_score1(m2, m3, m4, xx, 0, p2, p3, p4, xx, 0, s2, s3, s4, xx, 0, wp, wp, wp, xx, 0, m1, wp, 0, ws, wt, 1,
                    30,
                    "sanshoku ");  // 三色同順
  _test_calc_score1(m1, m2, m3, xx, 0, m7, m8, m9, xx, 0, m4, m5, m6, xx, 0, wp, wp, wp, xx, 0, s5, wp, 0, ws, wt, 1,
                    30,
                    "ittsu ");  // 一気通貫
  _test_calc_score1(m1, m2, m3, xx, 0, p7, p8, p9, xx, 0, m4, m5, m6, xx, 0, wp, wp, wp, xx, 0, s5, wp, 0, ws, wt, 0,
                    30,
                    "");  // 役なし(x一気通貫)
  _test_calc_score1(m1, m2, m3, xx, 0, p7, p8, p9, xx, 0, s1, s2, s3, xx, 0, wp, wp, wp, xx, 0, s1, wp, 0, ws, wt, 1,
                    30,
                    "chanta ");  // 混全帯么九
  _test_calc_score1(m1, m1, m1, xx, 0, p7, p7, p7, xx, 0, s2, s2, s2, xx, 0, s4, s4, s4, xx, 0, s3, p7, 1, ws, wt, 2,
                    30,
                    "toitoi ");  // 対々和(x三暗刻)
  _test_calc_score1(m1, m1, m1, xx, 0, p7, p7, p7, xx, 0, s2, s2, s2, xx, 0, s4, s4, s4, xx, 0, s3, p7, 0, ws, wt, 2,
                    40,
                    "toitoi ");  // 対々和(x三暗刻)(自摸+2符)
  _test_calc_score1(m1, m1, m1, xx, 0, p7, p8, p9, xx, 0, s2, s2, s2, xx, 0, s4, s4, s4, xx, 0, s3, p7, 0, ws, wt, 0,
                    30,
                    "");  // 役なし(x三暗刻)
  _test_calc_score1(m2, m2, m2, xx, 0, p7, p8, p9, xx, 0, s2, s2, s2, xx, 0, p2, p2, p2, xx, 0, s3, p2, 0, ws, wt, 2,
                    30,
                    "sanshoku_douko ");  // 三色同刻
  _test_calc_score1(m2, m2, m2, m2, 0, p7, p8, p9, xx, 0, s2, s2, s2, s2, 0, p3, p3, p3, p3, 0, s3, p3, 0, ws, wt, 2,
                    50,
                    "sankantsu ");  // 三槓子
  _test_calc_score1(dw, dw, dw, xx, 0, dr, dr, dr, xx, 0, s3, s4, s5, xx, 0, p2, p2, p2, xx, 0, dg, dr, 0, ws, wt, 4,
                    40,
                    "shosangen haku chun ");  // 小三元,白,中
  _test_calc_score1(m1, m1, m1, xx, 0, m9, m9, m9, xx, 0, s1, s1, s1, xx, 0, wp, wp, wp, xx, 0, dg, m1, 0, ws, wt, 4,
                    40,
                    "toitoi honroto ");  // 対々和,(x三暗刻),混老頭
  _test_calc_score1(m1, m1, m1, xx, 0, m6, m7, m8, xx, 0, s1, s1, s1, xx, 0, wt, wt, wt, xx, 0, m2, m1, 0, wt, wt, 2,
                    40,
                    "double_ton ");  // ダブ東
  _test_calc_score1(m1, m1, m1, xx, 0, m6, m7, m8, xx, 0, s1, s1, s1, xx, 0, wn, wn, wn, xx, 0, m2, m1, 0, wn, wn, 2,
                    40,
                    "double_nan ");  // ダブ南
  _test_calc_score1(m1, m1, m1, xx, 0, m6, m7, m8, xx, 0, s1, s1, s1, xx, 0, ws, ws, ws, xx, 0, m2, m1, 0, ws, ws, 2,
                    40,
                    "double_sha ");  // ダブ西
  _test_calc_score1(m1, m1, m1, xx, 0, m6, m7, m8, xx, 0, s1, s1, s1, xx, 0, wp, wp, wp, xx, 0, m2, m1, 0, wp, wp, 2,
                    40,
                    "double_pei ");  // ダブ北
  /*** 3翻(食い下がり2翻) ***/
  _test_calc_score1(m1, m1, m1, xx, 0, m6, m7, m8, xx, 0, m5, m6, m7, xx, 0, wp, wp, wp, xx, 0, m2, m1, 0, wt, wt, 2,
                    30,
                    "honitsu ");  // 混一色
  _test_calc_score1(m1, m1, m1, xx, 0, m7, m8, m9, xx, 0, p1, p2, p3, xx, 0, p9, p9, p9, xx, 0, s9, m1, 0, wt, wt, 2,
                    30,
                    "junchan ");  // 純全帯么九
  /*** 3翻 ***/
  _test_calc_score1(m2, m3, m4, xx, 0, p7, p8, p9, xx, 0, m2, m3, m4, xx, 0, p7, p8, p9, xx, 0, m2, m3, 0, wt, wt, 0,
                    30,
                    "");  // 役なし(x二盃口)
  _test_calc_score1(m2, m3, m4, xx, 0, p7, p8, p9, xx, 0, m2, m3, m4, xx, 0, p7, p8, p9, xx, 0, m2, m2, 0, wt, wt, 0,
                    30,
                    "");  // 役なし(x二盃口,x平和)
  /*** 6翻(食い下がり5翻) ***/
  _test_calc_score1(m2, m3, m4, xx, 0, m3, m4, m5, xx, 0, m5, m5, m5, xx, 0, m7, m8, m9, xx, 0, m1, m9, 0, wt, wt, 5,
                    30,
                    "chinitsu ");  // 清一色
  /*** 役満 ***/
  _test_calc_score1(s2, s2, s2, s2, 1, m3, m3, m3, m3, 1, wt, wt, wt, wt, 1, dg, dg, dg, dg, 1, m1, m1, 0, wt, wt, 26,
                    120,
                    "suuankou suukantsu ");  // 四暗刻,四槓子
  _test_calc_score1(dw, dw, dw, dw, 0, dr, dr, dr, xx, 0, s3, s4, s5, xx, 0, dg, dg, dg, xx, 0, m1, s3, 0, wt, wt, 13,
                    50,
                    "daisangen ");  // 大三元
  _test_calc_score1(s2, s2, s2, xx, 0, s2, s3, s4, xx, 0, s6, s6, s6, xx, 0, dg, dg, dg, xx, 0, s8, dg, 0, wt, wt, 13,
                    30,
                    "ryuisou ");  // 緑一色
  _test_calc_score1(wt, wt, wt, xx, 0, dw, dw, dw, xx, 0, dg, dg, dg, xx, 0, wn, wn, wn, xx, 0, wp, wn, 0, wt, wt, 13,
                    40,
                    "tsuisou ");  // 字一色
  _test_calc_score1(wt, wt, wt, xx, 0, ws, ws, ws, xx, 0, s7, s8, s9, xx, 0, wn, wn, wn, xx, 0, wp, wn, 0, wt, wt, 13,
                    40,
                    "shosuushi ");  // 小四喜
  _test_calc_score1(wt, wt, wt, xx, 0, ws, ws, ws, xx, 0, wp, wp, wp, xx, 0, wn, wn, wn, xx, 0, m2, wn, 0, wt, wt, 13,
                    40,
                    "daisuushi ");  // 大四喜
  _test_calc_score1(s9, s9, s9, xx, 0, s1, s1, s1, xx, 0, p1, p1, p1, xx, 0, p9, p9, p9, xx, 0, m1, p9, 0, wt, wt, 13,
                    40,
                    "chinroto ");  // 清老頭
  _test_calc_score1(s8, s8, s8, s8, 0, s1, s1, s1, s1, 0, p1, p1, p1, p1, 0, p9, p9, p9, p9, 0, m1, p9, 0, wt, wt, 13,
                    80,
                    "suukantsu ");  // 四槓子
  _test_calc_score1(wt, wt, wt, wt, 0, dw, dw, dw, xx, 0, dg, dg, dg, xx, 0, dr, dr, dr, xx, 0, wp, dr, 0, wt, wt, 26,
                    50,
                    "daisangen tsuisou ");  // 大三元,字一色
  _test_calc_score1(wt, wt, wt, wt, 1, dw, dw, dw, dw, 1, dg, dg, dg, dg, 1, dr, dr, dr, dr, 1, wp, wp, 0, wt, wt, 52,
                    160,
                    "suuankou daisangen tsuisou suukantsu ");  // 四暗刻,大三元,字一色,四槓子
  /*                 1, 2, 3, 4, c,  1, 2, 3, 4, c,  1, 2, 3, 4, c,  1, 2, 3, 4, c,pair,win,ron, pw, rw,han, fu, yaku_name */
}

static void _test_get_score(uint32_t han, uint32_t fu, bool tsumo, bool dealer, uint32_t scoreExp,
                            uint32_t scoreDealerExp) {
  uint32_t score = (uint32_t)-1;
  uint32_t scoreDealer = (uint32_t)-1;
  get_score(fu, han, tsumo, dealer, &score, &scoreDealer);
  fprintf(stderr, "han %d, fu %d, tsumo %d, dealer %d, score %d, scoreDealer %d\n", han, fu, tsumo, dealer, score,
          scoreDealer);
  assert(score == scoreExp);
  if (scoreDealerExp != (uint32_t)-1) {
    assert(scoreDealer == scoreDealerExp);
  }
}

void test_get_score() {
  // ロン, 子
  _test_get_score(1, 30, false, false, 1000, (uint32_t)-1);
  _test_get_score(1, 40, false, false, 1300, (uint32_t)-1);
  _test_get_score(1, 50, false, false, 1600, (uint32_t)-1);
  _test_get_score(1, 60, false, false, 2000, (uint32_t)-1);
  _test_get_score(1, 70, false, false, 2300, (uint32_t)-1);
  _test_get_score(1, 80, false, false, 2600, (uint32_t)-1);
  _test_get_score(1, 90, false, false, 2900, (uint32_t)-1);
  _test_get_score(1, 100, false, false, 3200, (uint32_t)-1);
  _test_get_score(1, 110, false, false, 3600, (uint32_t)-1);
  _test_get_score(2, 25, false, false, 1600, (uint32_t)-1);
  _test_get_score(2, 30, false, false, 2000, (uint32_t)-1);
  _test_get_score(2, 40, false, false, 2600, (uint32_t)-1);
  _test_get_score(3, 25, false, false, 3200, (uint32_t)-1);
  _test_get_score(3, 30, false, false, 3900, (uint32_t)-1);
  _test_get_score(3, 40, false, false, 5200, (uint32_t)-1);
  _test_get_score(3, 60, false, false, 8000, (uint32_t)-1);  // みなし満貫
  _test_get_score(3, 110, false, false, 8000, (uint32_t)-1);
  _test_get_score(4, 25, false, false, 6400, (uint32_t)-1);
  _test_get_score(4, 30, false, false, 8000, (uint32_t)-1);  // みなし満貫
  _test_get_score(4, 110, false, false, 8000, (uint32_t)-1);
  _test_get_score(5, 0, false, false, 8000, (uint32_t)-1);
  _test_get_score(6, 0, false, false, 12000, (uint32_t)-1);
  _test_get_score(7, 0, false, false, 12000, (uint32_t)-1);
  _test_get_score(8, 0, false, false, 16000, (uint32_t)-1);
  _test_get_score(9, 0, false, false, 16000, (uint32_t)-1);
  _test_get_score(10, 0, false, false, 16000, (uint32_t)-1);
  _test_get_score(11, 0, false, false, 24000, (uint32_t)-1);
  _test_get_score(12, 0, false, false, 24000, (uint32_t)-1);
  _test_get_score(13, 0, false, false, 32000, (uint32_t)-1);
  // ツモ, 子
  _test_get_score(1, 30, true, false, 300, 500);
  _test_get_score(1, 40, true, false, 400, 700);
  _test_get_score(1, 50, true, false, 400, 800);
  _test_get_score(1, 60, true, false, 500, 1000);
  _test_get_score(1, 70, true, false, 600, 1200);
  _test_get_score(1, 80, true, false, 700, 1300);
  _test_get_score(1, 90, true, false, 800, 1500);
  _test_get_score(1, 100, true, false, 800, 1600);
  _test_get_score(1, 110, true, false, 900, 1800);
  _test_get_score(2, 20, true, false, 400, 700);
  _test_get_score(2, 30, true, false, 500, 1000);
  _test_get_score(2, 40, true, false, 700, 1300);
  _test_get_score(2, 50, true, false, 800, 1600);
  _test_get_score(2, 60, true, false, 1000, 2000);
  _test_get_score(2, 70, true, false, 1200, 2300);
  _test_get_score(2, 80, true, false, 1300, 2600);
  _test_get_score(3, 20, true, false, 700, 1300);
  _test_get_score(3, 25, true, false, 800, 1600);
  _test_get_score(3, 30, true, false, 1000, 2000);
  _test_get_score(3, 40, true, false, 1300, 2600);
  _test_get_score(3, 60, true, false, 2000, 4000);  // みなし満貫
  _test_get_score(3, 100, true, false, 2000, 4000);
  _test_get_score(4, 20, true, false, 1300, 2600);
  _test_get_score(4, 25, true, false, 1600, 3200);
  _test_get_score(4, 30, true, false, 2000, 4000);  // みなし満貫
  _test_get_score(4, 110, true, false, 2000, 4000);
  _test_get_score(5, 0, true, false, 2000, 4000);
  _test_get_score(6, 0, true, false, 3000, 6000);
  _test_get_score(7, 0, true, false, 3000, 6000);
  _test_get_score(8, 0, true, false, 4000, 8000);
  _test_get_score(9, 0, true, false, 4000, 8000);
  _test_get_score(10, 0, true, false, 4000, 8000);
  _test_get_score(11, 0, true, false, 6000, 12000);
  _test_get_score(12, 0, true, false, 6000, 12000);
  _test_get_score(13, 0, true, false, 8000, 16000);
  // ロン, 親
  _test_get_score(1, 30, false, true, 1500, (uint32_t)-1);
  _test_get_score(1, 40, false, true, 2000, (uint32_t)-1);
  _test_get_score(1, 50, false, true, 2400, (uint32_t)-1);
  _test_get_score(1, 60, false, true, 2900, (uint32_t)-1);
  _test_get_score(1, 70, false, true, 3400, (uint32_t)-1);
  _test_get_score(1, 80, false, true, 3900, (uint32_t)-1);
  _test_get_score(1, 90, false, true, 4400, (uint32_t)-1);
  _test_get_score(1, 100, false, true, 4800, (uint32_t)-1);
  _test_get_score(1, 110, false, true, 5300, (uint32_t)-1);
  _test_get_score(2, 25, false, true, 2400, (uint32_t)-1);
  _test_get_score(2, 30, false, true, 2900, (uint32_t)-1);
  _test_get_score(2, 40, false, true, 3900, (uint32_t)-1);
  _test_get_score(2, 50, false, true, 4800, (uint32_t)-1);
  _test_get_score(2, 60, false, true, 5800, (uint32_t)-1);
  _test_get_score(2, 70, false, true, 6800, (uint32_t)-1);
  _test_get_score(2, 80, false, true, 7700, (uint32_t)-1);
  _test_get_score(2, 110, false, true, 10600, (uint32_t)-1);
  _test_get_score(3, 25, false, true, 4800, (uint32_t)-1);
  _test_get_score(3, 30, false, true, 5800, (uint32_t)-1);
  _test_get_score(3, 40, false, true, 7700, (uint32_t)-1);
  _test_get_score(3, 50, false, true, 9600, (uint32_t)-1);
  _test_get_score(3, 60, false, true, 12000, (uint32_t)-1);  // みなし満貫
  _test_get_score(3, 100, false, true, 12000, (uint32_t)-1);
  _test_get_score(4, 25, false, true, 9600, (uint32_t)-1);
  _test_get_score(4, 30, false, true, 12000, (uint32_t)-1);  // みなし満貫
  _test_get_score(4, 100, false, true, 12000, (uint32_t)-1);
  _test_get_score(5, 0, false, true, 12000, (uint32_t)-1);
  _test_get_score(6, 0, false, true, 18000, (uint32_t)-1);
  _test_get_score(7, 0, false, true, 18000, (uint32_t)-1);
  _test_get_score(8, 0, false, true, 24000, (uint32_t)-1);
  _test_get_score(9, 0, false, true, 24000, (uint32_t)-1);
  _test_get_score(10, 0, false, true, 24000, (uint32_t)-1);
  _test_get_score(11, 0, false, true, 36000, (uint32_t)-1);
  _test_get_score(12, 0, false, true, 36000, (uint32_t)-1);
  _test_get_score(13, 0, false, true, 48000, (uint32_t)-1);
  // ツモ, 親
  _test_get_score(1, 30, true, true, 500, (uint32_t)-1);
  _test_get_score(1, 40, true, true, 700, (uint32_t)-1);
  _test_get_score(1, 50, true, true, 800, (uint32_t)-1);
  _test_get_score(1, 60, true, true, 1000, (uint32_t)-1);
  _test_get_score(1, 70, true, true, 1200, (uint32_t)-1);
  _test_get_score(1, 80, true, true, 1300, (uint32_t)-1);
  _test_get_score(1, 90, true, true, 1500, (uint32_t)-1);
  _test_get_score(1, 100, true, true, 1600, (uint32_t)-1);
  _test_get_score(1, 110, true, true, 1800, (uint32_t)-1);
  _test_get_score(2, 20, true, true, 700, (uint32_t)-1);
  _test_get_score(2, 30, true, true, 1000, (uint32_t)-1);
  _test_get_score(2, 40, true, true, 1300, (uint32_t)-1);
  _test_get_score(2, 50, true, true, 1600, (uint32_t)-1);
  _test_get_score(2, 60, true, true, 2000, (uint32_t)-1);
  _test_get_score(2, 70, true, true, 2300, (uint32_t)-1);
  _test_get_score(2, 80, true, true, 2600, (uint32_t)-1);
  _test_get_score(2, 90, true, true, 2900, (uint32_t)-1);
  _test_get_score(2, 110, true, true, 3600, (uint32_t)-1);
  _test_get_score(3, 20, true, true, 1300, (uint32_t)-1);
  _test_get_score(3, 25, true, true, 1600, (uint32_t)-1);
  _test_get_score(3, 30, true, true, 2000, (uint32_t)-1);
  _test_get_score(3, 40, true, true, 2600, (uint32_t)-1);
  _test_get_score(3, 50, true, true, 3200, (uint32_t)-1);
  _test_get_score(3, 60, true, true, 4000, (uint32_t)-1);  // みなし満貫
  _test_get_score(3, 100, true, true, 4000, (uint32_t)-1);
  _test_get_score(4, 20, true, true, 2600, (uint32_t)-1);
  _test_get_score(4, 25, true, true, 3200, (uint32_t)-1);
  _test_get_score(4, 30, true, true, 4000, (uint32_t)-1);  // みなし満貫
  _test_get_score(4, 110, true, true, 4000, (uint32_t)-1);
  _test_get_score(5, 0, true, true, 4000, (uint32_t)-1);
  _test_get_score(6, 0, true, true, 6000, (uint32_t)-1);
  _test_get_score(7, 0, true, true, 6000, (uint32_t)-1);
  _test_get_score(8, 0, true, true, 8000, (uint32_t)-1);
  _test_get_score(9, 0, true, true, 8000, (uint32_t)-1);
  _test_get_score(10, 0, true, true, 8000, (uint32_t)-1);
  _test_get_score(11, 0, true, true, 12000, (uint32_t)-1);
  _test_get_score(12, 0, true, true, 12000, (uint32_t)-1);
  _test_get_score(13, 0, true, true, 16000, (uint32_t)-1);
}

bool test_score() {
  test_calc_score_with_tiles();
  test_calc_score();
  test_get_score();
  return true;
}
