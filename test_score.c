#include <stdio.h>
#include <assert.h>

#include "test_score.h"
#include "test_util.h"

static void _test_calc_score_concealed_hands(
        /* input */
        MJTileId t1,
        MJTileId t2,
        MJTileId t3,
        MJTileId t4,
        MJTileId t5,
        MJTileId t6,
        MJTileId t7,
        MJTileId t8,
        MJTileId t9,
        MJTileId t10,
        MJTileId t11,
        MJTileId t12,
        MJTileId pair,
        MJTileId win_tile,
        bool ron,
        MJTileId player_wind,
        MJTileId round_wind,
        /* expected */
        bool retval,
        uint32_t han,
        uint32_t fu,
        const char *yaku_name) {
    MJScore score;
    memset(&score, -1, sizeof(MJScore));
    Tiles tiles;
    MJHands hands = {
        {t1,t2,t3, t4,t5,t6, t7,t8,t9, t10,t11,t12, pair,pair},
        3 * 4 + 2,
    };
    assert(gen_tiles_from_hands(&tiles, &hands));

    ScoreConfig cfg = {win_tile, ron, player_wind, round_wind};
    assert(calc_score_concealed_hands(&score, &tiles, &cfg) == retval);
    if (retval) {
        assert(score.han == han);
        assert(score.fu == fu);
        assert(strcmp(score.yaku_name, yaku_name) == 0);
    }
}

void test_calc_score_concealed_hands() {
    /*                                1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,pa,win,ron,pw,rw, ret, han,fu, yaku_name */
    _test_calc_score_concealed_hands(dw,dg,dr,m1,p1,s1,m9,p9,s9,wt,wn,ws,wp, dr, 1, wt,wt, true, 13, 0, "kokushi ");
    _test_calc_score_concealed_hands(m1,m1,m2,m2,m3,m3,p3,p3,p4,p4,p5,p5,wt, wt, 1, wt,wt, true, 2, 25, "chiitoitsu ");
    _test_calc_score_concealed_hands(m1,m1,m2,m2,m3,m3,p3,p3,p4,p4,p5,p5,wt, wt, 0, wt,wt, true, 3, 25, "chiitoitsu tsumo ");
    _test_calc_score_concealed_hands(s2,s2,m2,m2,m3,m3,p3,p3,p4,p4,p5,p5,p6, p5, 1, wt,wt, true, 3, 25, "chiitoitsu tanyao ");
    _test_calc_score_concealed_hands(wt,wt,wn,wn,ws,ws,wp,wp,dw,dw,dg,dg,dr, dr, 1, wt,wt, true, 13, 25, "tsuisou ");
    _test_calc_score_concealed_hands(p1,p1,p3,p3,p4,p4,p6,p6,wt,wt,wn,wn,wp, wp, 1, wt,wt, true, 5, 25, "chiitoitsu honitsu ");
    _test_calc_score_concealed_hands(p1,p1,p3,p3,p4,p4,p6,p6,p7,p7,p8,p8,p9, p1, 1, wt,wt, true, 8, 25, "chiitoitsu chinitsu ");
    _test_calc_score_concealed_hands(p1,p1,p9,p9,m1,m1,s1,s1,wt,wt,dw,dw,dr, dr, 1, wt,wt, true, 4, 25, "chiitoitsu honroto ");
    _test_calc_score_concealed_hands(p1,p1,p9,p9,m1,m1,s1,s1,wt,wt,dw,dw,dr, dr, 1, wt,wt, true, 4, 25, "chiitoitsu honroto ");
    _test_calc_score_concealed_hands(p1,p1,p9,p9,m1,m1,s1,s1,wt,wt,dw,dw,m1, dr, 1, wt,wt, false, 0, 0, ""); // non chiitoitsu
}

static void _test_calc_score0( /* 門前 */
        /* input */
        MJTileId e11,
        MJTileId e12,
        MJTileId e13,
        MJTileId e21,
        MJTileId e22,
        MJTileId e23,
        MJTileId e31,
        MJTileId e32,
        MJTileId e33,
        MJTileId e41,
        MJTileId e42,
        MJTileId e43,
        MJTileId pair,
        MJTileId win_tile,
        bool ron,
        MJTileId player_wind,
        MJTileId round_wind,
        /* expected */
        uint32_t han,
        uint32_t fu,
        const char *yaku_name) {
    MJScore score;
    memset(&score, -1, sizeof(MJScore));
    Elements concealed;
    Elements melded;
    MJMelds melds = {
        {
            {{e11,e12,e13}, 3, true, -1},
            {{e21,e22,e23}, 3, true, -1},
            {{e31,e32,e33}, 3, true, -1},
            {{e41,e42,e43}, 3, true, -1},
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

void test_calc_score() {
    /*                                                       pair,win,ron, pw, rw,han, fu, yaku_name */
    /*** 1翻 ***/
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, s7,s8,s9,  m1, s9,  1, wt, wt,  1, 30, "pinfu "); // 平和(ロン)
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, s7,s8,s9,  m1, s9,  0, wt, wt,  2, 20, "pinfu tsumo "); // 平和(自摸)
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, s7,s8,s9,  m1, m3,  1, wt, wt,  0, 40, ""); // 役無し(ペンチャン待ち)
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, s7,s8,s9,  m1, p3,  1, wt, wt,  0, 40, ""); // 役無し(カンチャン待ち)
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, s7,s8,s9,  m1, m3,  0, wt, wt,  1, 30, "tsumo "); // 自摸
    _test_calc_score0(m2,m3,m4, p2,p3,p4, s3,s4,s5, s6,s7,s8,  m2, p3,  1, wt, wt,  1, 40, "tanyao "); // 断么九,
    _test_calc_score0(m2,m3,m4, p2,p3,p4, p2,p3,p4, s6,s7,s8,  m2, p4,  1, wt, wt,  3, 30, "pinfu tanyao iipeiko "); // 平和,断么九,一盃口
    _test_calc_score0(m1,m1,m1, p2,p3,p4, p2,p3,p4, s6,s7,s8,  m2, p4,  1, wt, wt,  1, 40, "iipeiko "); // 一盃口
    _test_calc_score0(m1,m1,m1, p2,p3,p4, p2,p3,p4, dw,dw,dw,  dw, p4,  1, wt, wt,  2, 50, "iipeiko haku "); // 一盃口,白
    _test_calc_score0(m1,m2,m3, p2,p3,p4, dg,dg,dg, dw,dw,dw,  m9, p3,  2, wt, wt,  2, 50, "haku hatsu "); // 白,發
    _test_calc_score0(m1,m2,m3, dr,dr,dr, p3,p3,p3, dw,dw,dw,  m9, p3,  2, wt, wt,  2, 50, "haku chun "); // 白,中
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, wt,wt,wt,  m1, wt,  1, wt, wn,  1, 40, "ton "); // 東
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, wn,wn,wn,  m1, wn,  1, wt, wn,  1, 40, "nan "); // 南
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, ws,ws,ws,  m1, ws,  1, ws, wn,  1, 40, "sha "); // 西
    _test_calc_score0(m1,m2,m3, p2,p3,p4, s3,s4,s5, wp,wp,wp,  m1, wp,  1, ws, wp,  1, 40, "pei "); // 北
    /*** 2翻(食い下がり1翻) ***/
    _test_calc_score0(m2,m3,m4, p2,p3,p4, s2,s3,s4, wp,wp,wp,  m1, wp,  1, ws, wt,  2, 40, "sanshoku "); // 三色同順
    _test_calc_score0(m1,m2,m3, m7,m8,m9, m4,m5,m6, wp,wp,wp,  s5, wp,  1, ws, wt,  2, 40, "ittsu "); // 一気通貫
    _test_calc_score0(m1,m2,m3, p7,p8,p9, m4,m5,m6, wp,wp,wp,  s5, wp,  1, ws, wt,  0, 40, ""); // (一気通貫テスト)
    _test_calc_score0(m1,m2,m3, p7,p8,p9, s1,s2,s3, wp,wp,wp,  s1, wp,  1, ws, wt,  2, 40, "chanta "); // 混全帯么九
    /*** 2翻 ***/
    _test_calc_score0(m1,m1,m1, p7,p7,p7, s2,s2,s2, s4,s4,s4,  s3, p7,  1, ws, wt,  4, 50, "toitoi sanankou "); // 対々和,三暗刻
    _test_calc_score0(m1,m1,m1, p7,p8,p9, s2,s2,s2, s4,s4,s4,  s3, p7,  1, ws, wt,  2, 50, "sanankou "); // 三暗刻
    _test_calc_score0(m2,m2,m2, p7,p8,p9, s2,s2,s2, p2,p2,p2,  s3, p2,  1, ws, wt,  2, 40, "sanshoku_douko "); // 三色同刻(p2が明刻扱いでちょうど40符)
    // TODO: 三槓子
    _test_calc_score0(dw,dw,dw, dr,dr,dr, s3,s4,s5, p2,p2,p2,  dg, dr,  1, ws, wt,  4, 50, "shosangen haku chun "); // 小三元,白,中
    _test_calc_score0(m1,m1,m1, m9,m9,m9, s1,s1,s1, wp,wp,wp,  dg, m1,  1, ws, wt,  6, 60, "toitoi sanankou honroto "); // 対々和,三暗刻,混老頭
    _test_calc_score0(m1,m1,m1, m6,m7,m8, s1,s1,s1, wt,wt,wt,  m2, m1,  1, wt, wt,  2, 50, "double_ton "); // ダブ東
    _test_calc_score0(m1,m1,m1, m6,m7,m8, s1,s1,s1, wn,wn,wn,  m2, m1,  1, wn, wn,  2, 50, "double_nan "); // ダブ南
    _test_calc_score0(m1,m1,m1, m6,m7,m8, s1,s1,s1, ws,ws,ws,  m2, m1,  1, ws, ws,  2, 50, "double_sha "); // ダブ西
    _test_calc_score0(m1,m1,m1, m6,m7,m8, s1,s1,s1, wp,wp,wp,  m2, m1,  1, wp, wp,  2, 50, "double_pei "); // ダブ北
    _test_calc_score0(m1,m1,m1, m6,m7,m8, s1,s1,s1, wp,wp,wp,  m2, m1,  1, wp, wp,  2, 50, "double_pei "); // ダブ北
    /*** 3翻(食い下がり2翻) ***/
    _test_calc_score0(m1,m1,m1, m6,m7,m8, m5,m6,m7, wp,wp,wp,  m2, m1,  1, wt, wt,  3, 50, "honitsu "); // 混一色
    _test_calc_score0(m1,m1,m1, m7,m8,m9, p1,p2,p3, p9,p9,p9,  s9, m1,  1, wt, wt,  3, 50, "junchan "); // 純全帯么九
    /*** 3翻 ***/
    _test_calc_score0(m2,m3,m4, p7,p8,p9, m2,m3,m4, p7,p8,p9,  m2, m3,  1, wt, wt,  3, 40, "ryanpeiko "); // 二盃口
    _test_calc_score0(m2,m3,m4, p7,p8,p9, m2,m3,m4, p7,p8,p9,  m2, m2,  1, wt, wt,  4, 30, "ryanpeiko pinfu "); // 二盃口,平和
    /*** 6翻(食い下がり5翻) ***/
    _test_calc_score0(m2,m3,m4, m3,m4,m5, m5,m5,m5, m7,m8,m9,  m1, m9,  1, wt, wt,  6, 40, "chinitsu "); // 清一色
    /*** 役満 ***/
    _test_calc_score0(m2,m2,m2, p3,p3,p3, m5,m5,m5, m7,m7,m7,  m1, m7,  1, wt, wt,  4, 50, "toitoi sanankou "); // 対々和,三暗刻(四暗刻テスト)
    _test_calc_score0(m2,m2,m2, p3,p3,p3, m5,m5,m5, m7,m7,m7,  m1, m1,  1, wt, wt, 13, 50, "suuankou "); // 四暗刻
    _test_calc_score0(dw,dw,dw, dr,dr,dr, s3,s4,s5, dg,dg,dg,  m1, s3,  1, ws, wt, 13, 60, "daisangen "); // 大三元
    _test_calc_score0(s2,s2,s2, s2,s3,s4, s6,s6,s6, dg,dg,dg,  s8, dg,  1, ws, wt, 13, 50, "ryuisou "); // 緑一色
    _test_calc_score0(wt,wt,wt, dw,dw,dw, dg,dg,dg, wn,wn,wn,  wp, wn,  1, ws, wt, 13, 60, "tsuisou "); // 字一色
    _test_calc_score0(wt,wt,wt, ws,ws,ws, s7,s8,s9, wn,wn,wn,  wp, wn,  1, ws, wt, 13, 50, "shosuushi "); // 小四喜
    _test_calc_score0(wt,wt,wt, ws,ws,ws, wp,wp,wp, wn,wn,wn,  m2, wn,  1, ws, wt, 13, 60, "daisuushi "); // 大四喜
    _test_calc_score0(s9,s9,s9, s1,s1,s1, p1,p1,p1, p9,p9,p9,  m1, p9,  1, ws, wt, 13, 60, "chinroto "); // 清老頭
    // TODO: 四槓子
    _test_calc_score0(m1,m1,m1, m1,m2,m3, m4,m5,m6, m7,m8,m9,  m9, m9,  1, ws, wt, 13, 40, "chuuren_poutou "); // 九蓮宝燈(x=1)
    _test_calc_score0(m1,m1,m1, m3,m4,m5, m6,m7,m8, m9,m9,m9,  m2, m9,  1, ws, wt, 13, 50, "chuuren_poutou "); // 九蓮宝燈(x=2)
    _test_calc_score0(m1,m2,m3, m3,m4,m5, m6,m7,m8, m9,m9,m9,  m1, m9,  1, ws, wt, 13, 40, "chuuren_poutou "); // 九蓮宝燈(x=3)
    _test_calc_score0(m1,m1,m1, m2,m3,m4, m4,m5,m6, m7,m8,m9,  m9, m9,  1, ws, wt, 13, 40, "chuuren_poutou "); // 九蓮宝燈(x=4)
    _test_calc_score0(m1,m1,m1, m2,m3,m4, m6,m7,m8, m9,m9,m9,  m5, m9,  1, ws, wt, 13, 50, "chuuren_poutou "); // 九蓮宝燈(x=5)
    _test_calc_score0(m1,m2,m3, m4,m5,m6, m6,m7,m8, m9,m9,m9,  m1, m9,  1, ws, wt, 13, 40, "chuuren_poutou "); // 九蓮宝燈(x=6)
    _test_calc_score0(m1,m1,m1, m2,m3,m4, m5,m6,m7, m7,m8,m9,  m9, m9,  1, ws, wt, 13, 40, "chuuren_poutou "); // 九蓮宝燈(x=7)
    _test_calc_score0(m1,m1,m1, m2,m3,m4, m5,m6,m7, m9,m9,m9,  m8, m9,  1, ws, wt, 13, 50, "chuuren_poutou "); // 九蓮宝燈(x=8)
    _test_calc_score0(m1,m2,m3, m4,m5,m6, m7,m8,m9, m9,m9,m9,  m1, m9,  1, ws, wt, 13, 40, "chuuren_poutou "); // 九蓮宝燈(x=9)
    /*                                                       pair,win,ron, pw, rw,han, fu, yaku_name */
}

bool test_score() {
    test_calc_score_concealed_hands();
    test_calc_score();
    return true;
}