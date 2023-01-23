#include <stdio.h>
#include <assert.h>

#include "test_mahjong.h"
#include "test_util.h"

static void _test_mj_get_score(
        // input
        MJTileId h01,
        MJTileId h02,
        MJTileId h03,
        MJTileId h04,
        MJTileId h05,
        MJTileId h06,
        MJTileId h07,
        MJTileId h08,
        MJTileId h09,
        MJTileId h10,
        MJTileId h11,
        MJTileId h12,
        MJTileId h13,
        MJTileId h14,
        MJTileId win_tile,
        bool ron,
        MJTileId player_wind,
        MJTileId round_wind,
        // expected
        uint32_t han,
        uint32_t fu,
        const char *yaku_name) {
    MJScore score;
    MJHands hands = {{h01,h02,h03,h04,h05,h06,h07,h08,h09,h10,h11,h12,h13,h14}, 14};
    MJMelds melds = {{},0};
    mj_get_score(&score, &hands, &melds, win_tile, ron, player_wind, round_wind);
    assert(score.han == han);
    assert(score.fu == fu);
    assert(strcmp(score.yaku_name, yaku_name) == 0);
}

void test_mj_get_score() {
    _test_mj_get_score(m1,m2,m3,p1,p2,p3,s1,s2,s3,s1,s2,s3,s9,s9, p1, 1, wt, wt, 7, 30, "junchan sanshoku pinfu iipeiko ");
    _test_mj_get_score(m1,m2,m3,p1,p2,p3,wn,wn,wn,s2,s3,s4,s9,s9, p1, 1, wt, wt, 0, 40, ""); // 役なし
    // https://mj-dragon.com/calc/basic/kotenho.html
    _test_mj_get_score(m3,m3,m4,m4,m5,m5,p9,p9,s4,s4,s5,s5,s6,s6, s5, 1, wt, wt, 3, 40, "ryanpeiko ");
    _test_mj_get_score(m2,m3,m4,p2,p2,p3,p3,p4,p4,p5,p5,s2,s3,s4, p3, 1, wt, wt, 4, 40, "sanshoku tanyao iipeiko ");
    // https://mahjongcollege.com/score-calculation/7815/
    _test_mj_get_score(m5,m5,p2,p3,p4,s4,s5,s6,s6,s7,dw,dw,dw,s5, s5, 0, wt, wt, 2, 40, "haku tsumo ");
    // https://www.mahjong.or.jp/column/1176/
    _test_mj_get_score(m1,m1,m2,m3,p7,p7,p7,p8,p8,p8,p9,p9,p9,m1, m1, 1, wt, wt, 5, 30, "junchan pinfu iipeiko ");
}

bool test_mahjong() {
    test_mj_get_score();
    return true;
}
