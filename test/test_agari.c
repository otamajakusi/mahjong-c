#include "test_agari.h"

#include <assert.h>
#include <stdio.h>

#include "test_util.h"
#include "tile.h"

static bool cb_tiles(const Tiles *tiles, void *arg) {
  (void)tiles;
  (void)arg;
  return false;
}

static bool cb_elements(const Elements *concealed, const Elements *melded, MJTileId pair, void *arg) {
  (void)concealed;
  (void)melded;
  (void)pair;
  (void)arg;
  return false;
}

static uint32_t test_find_agari_menzen(MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6,
                                       MJTileId t7, MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12,
                                       MJTileId t13, MJTileId t14) {
  MJHands hands = {
      {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14},
      3 * 4 + 2,
  };
  Tiles tiles;
  assert(gen_tiles_from_hands(&tiles, &hands));
  Elements elems = {{}, 0};

  return find_agari(&tiles, &elems, cb_tiles, cb_elements, NULL);
}

void test_find_agari() {
  assert(test_find_agari_menzen(m1, m2, m3, m1, m2, m3, p1, p2, p3, s1, s2, s3, dw, dw) == 1);
  assert(test_find_agari_menzen(m1, m2, m3, m1, m2, m3, m1, m2, m3, s1, s2, s3, dg, dg) == 2);
  assert(test_find_agari_menzen(m1, m2, m3, m1, m2, m3, m1, m2, m3, m1, m2, m3, dr, dr) == 2);

  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m1) == 1);  // 九蓮宝燈
  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m2) == 1);  // 九蓮宝燈
  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m3) == 1);  // 九蓮宝燈
  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m4) == 1);  // 九蓮宝燈
  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m5) == 1);  // 九蓮宝燈
  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m6) == 1);  // 九蓮宝燈
  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m7) == 1);  // 九蓮宝燈
  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m8) == 1);  // 九蓮宝燈
  assert(test_find_agari_menzen(m1, m1, m1, m2, m3, m4, m5, m6, m7, m8, m9, m9, m9, m9) == 1);  // 九蓮宝燈

  // https://www.engineer-log.com/entry/2018/06/14/mahjong-algorithm
  assert(test_find_agari_menzen(p2, p3, p3, p3, p3, p4, p4, p4, p5, p5, p6, p6, p8, p8) == 1);
  assert(test_find_agari_menzen(p1, p1, p5, p5, p5, p6, p7, p7, p7, p8, p8, p8, p9, p9) == 1);

  // https://mj-dragon.com/calc/basic/kotenho.html
  assert(test_find_agari_menzen(m2, m3, m4, p2, p2, p3, p3, p4, p4, p5, p5, s2, s3, s4) == 2);  // シャンポン待ち
}

bool test_agari() {
  test_find_agari();
  return true;
}
