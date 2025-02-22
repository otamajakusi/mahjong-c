#include "test_shanten.h"

#include <assert.h>
#include <stdio.h>

#include "test_util.h"

int32_t test_calc_shanten_13(MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6, MJTileId t7,
                             MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12, MJTileId t13) {
  const MJHands hands1 = {
      {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13},
      3 * 4 + 1,
  };
  return mj_calc_shanten(&hands1);
}

void test_calc_shanten() {
  // テンパイ
  assert(test_calc_shanten_13(m1, m2, m3, m1, m2, m3, p1, p2, p3, s1, s2, s3, dw) == 0);
  assert(test_calc_shanten_13(m1, m2, m3, m3, m3, m4, m4, m5, m5, m6, m7, m8, dw) == 0);
  assert(test_calc_shanten_13(m1, m2, m3, m3, m3, m4, m4, m5, m5, m9, m9, m9, dw) == 0);
  assert(test_calc_shanten_13(m1, m1, m1, m3, m3, m4, m4, m5, m5, m9, m9, m9, dw) == 0);
  assert(test_calc_shanten_13(m3, m3, m3, m3, m4, m4, m4, m5, m5, m5, m6, m6, dw) == 0);
  // 1シャンテン
  // 刻子を単に抜くと駄目(333, 345, 456, 45, x, y), (333, 3, 444, 555, 6, x, y) FIXME: 面子だけ先に計算するのは駄目
  assert(test_calc_shanten_13(m3, m3, m3, m3, m4, m4, m4, m5, m5, m5, m6, p6, dw) == 1);
}

bool test_shanten() {
  test_calc_shanten();
  return true;
}
