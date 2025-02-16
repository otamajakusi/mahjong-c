#include "test_shanten.h"

#include <assert.h>
#include <stdio.h>

#include "test_util.h"

void test_calc_shanten() {
  const MJHands hands1 = {
      {m1, m2, m3, m4, m5, m6, m7, m8, m9, m7, m8, m9, dw},
      3 * 4 + 1,
  };
  mj_calc_shanten(&hands1);
}

bool test_shanten() {
  test_calc_shanten();
  return true;
}
