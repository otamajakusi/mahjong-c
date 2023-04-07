#include <stdio.h>

#include "mahjong.h"

int main() {
  MJHands hands = {{
                       MJ_M2, MJ_M3, MJ_M4,  // 萬2,萬3,萬4,
                       MJ_P2, MJ_P2, MJ_P3, MJ_P3, MJ_P4, MJ_P4, MJ_P5,
                       MJ_P5,                // 筒2,筒2,筒3,筒3,筒4,筒4,筒5,筒5,
                       MJ_S2, MJ_S3, MJ_S4,  // 索2,索3,索4,
                   },
                   14};
  MJMelds melds = {{}, 0};       // 副露(暗槓含む)牌=0組
  MJTileId win_tile = MJ_P3;     // アガリ牌=筒3
  bool ron = true;               // ロン=true
  MJTileId player_wind = MJ_WT;  // 自風=東
  MJTileId round_wind = MJ_WT;   // 場風=東
  MJBaseScore score;
  mj_get_score(&score, &hands, &melds, win_tile, ron, player_wind, round_wind);
  printf("han: %d, fu: %d, yaku: %s\n", score.han, score.fu, score.yaku_name);
  return 0;
}
