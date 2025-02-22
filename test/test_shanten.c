#include "test_shanten.h"

#include <assert.h>
#include <stdio.h>

#include "test_util.h"

void test_calc_shanten_13(MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6, MJTileId t7,
                          MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12, MJTileId t13,
                          int shanten) {
  const MJHands hands1 = {
      {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13},
      3 * 4 + 1,
  };
  assert(mj_calc_shanten(&hands1) == shanten);
}

void test_calc_shanten() {
  test_calc_shanten_13(m1, m2, m3, m1, m2, m3, p1, p2, p3, s1, s2, s3, dw, 0);
  test_calc_shanten_13(m1, m2, m3, m3, m3, m4, m4, m5, m5, m6, m7, m8, dw, 0);
  test_calc_shanten_13(m1, m2, m3, m3, m3, m4, m4, m5, m5, m9, m9, m9, dw, 0);
  test_calc_shanten_13(m1, m1, m1, m3, m3, m4, m4, m5, m5, m9, m9, m9, dw, 0);
  test_calc_shanten_13(m3, m3, m3, m3, m4, m4, m4, m5, m5, m5, m6, m6, dw, 0);
}

bool test_shanten() {
  test_calc_shanten();
  return true;
}
