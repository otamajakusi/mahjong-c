#include "test_shanten.h"

#include <assert.h>
#include <stdio.h>

#include "test_util.h"

#define SHOW_PROGRESS 0

int32_t test_calc_shanten_13(MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6, MJTileId t7,
                             MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12, MJTileId t13) {
  const MJHands hands1 = {
      {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13},
      3 * 4 + 1,
  };
  return mj_calc_shanten(&hands1);
}

int32_t test_calc_shanten_14(MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6, MJTileId t7,
                             MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12, MJTileId t13,
                             MJTileId t14) {
  const MJHands hands1 = {
      {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14},
      3 * 4 + 2,
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
  // 刻子を単に抜くと駄目(333, 345, 456, 45, x, y), (333, 3, 444, 555, 6, x, y)
  assert(test_calc_shanten_13(m3, m3, m3, m3, m4, m4, m4, m5, m5, m5, m6, p6, dw) == 1);

  /* 4 9 12 15 16 17 21 21 24 27 29 30 32 33 5 6 5 */
  assert(test_calc_shanten_14(4, 9, 12, 15, 16, 17, 21, 21, 24, 27, 29, 30, 32, 33) == 5);
}

void test_file(const char *file) {
  fprintf(stderr, "%s\n", file);
  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    fprintf(stderr, "failed to open file: %s\n", file);
    return;
  }

  char line[1024];
  int c = 0;
  while (fgets(line, sizeof(line), fp) != NULL) {
    MJHands hands;
    int32_t shanten_normal, shanten_kokushi, shanten_chitoi;
    sscanf(line, "%u %u %u %u %u %u %u %u %u %u %u %u %u %u %d %d %d", &hands.tile_id[0], &hands.tile_id[1],
           &hands.tile_id[2], &hands.tile_id[3], &hands.tile_id[4], &hands.tile_id[5], &hands.tile_id[6],
           &hands.tile_id[7], &hands.tile_id[8], &hands.tile_id[9], &hands.tile_id[10], &hands.tile_id[11],
           &hands.tile_id[12], &hands.tile_id[13], &shanten_normal, &shanten_kokushi, &shanten_chitoi);
    hands.len = 14;

    int32_t shanten = mj_calc_shanten(&hands);
    if (shanten != shanten_normal) {
      fprintf(stderr, "%s", line);
      fprintf(stderr, "shanten %d %d %d %d\n", shanten, shanten_normal, shanten_kokushi, shanten_chitoi);
    }
    assert(shanten == shanten_normal);
#if SHOW_PROGRESS
    fprintf(stderr, "%d\r", c);
#endif
    c++;
  }

  fclose(fp);
}

const char test_files[][32] = {"test/p_hon_10000.txt", "test/p_koku_10000.txt", "test/p_normal_10000.txt",
                               "test/p_tin_10000.txt"};

bool test_shanten() {
  test_calc_shanten();
  for (uint32_t i = 0; i < sizeof(test_files) / sizeof(test_files[0]); i++) {
    test_file(test_files[i]);
  }
  return true;
}
