#include "test_ukeire.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "test_util.h"
#include "tile.h"
#include "ukeire.h"

static void dump_tiles(const Tiles *tiles) {
  bool found = false;
  for (uint32_t i = MJ_M1; i <= MJ_DR; i++) {
    if (tiles->tiles[i]) {
      found = true;
      fprintf(stderr, "%s,", tile_id_str(i));
    }
  }
  if (found) {
    fprintf(stderr, "\n");
  }
}

static void init(ShantenCtx *ctx, MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6,
                 MJTileId t7, MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12, MJTileId t13) {
  const MJHands hands = {
      {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13},
      3 * 4 + 1,
  };
  init_ctx(ctx, &hands);
}

static int run_test(void (*gen_acceptable)(ShantenCtx *, Tiles *), MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4,
                    MJTileId t5, MJTileId t6, MJTileId t7, MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11,
                    MJTileId t12, MJTileId t13, int len, va_list args) {
  (void)dump_tiles;
  Tiles expect;
  memset(&expect, 0, sizeof(Tiles));
  for (int i = 0; i < len; i++) {
    int value = va_arg(args, int);
    expect.tiles[value] = 1;
  }

  ShantenCtx ctx;
  Tiles acceptables;
  init(&ctx, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
  gen_acceptable(&ctx, &acceptables);
  int n = memcmp(&acceptables, &expect, sizeof(Tiles));
  if (n != 0) {
    fprintf(stderr, "act\n");
    dump_tiles(&acceptables);
    fprintf(stderr, "exp\n");
    dump_tiles(&expect);
  }
  return n;
}

static int kokushi(MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6, MJTileId t7,
                   MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12, MJTileId t13, int len, ...) {
  va_list args;
  va_start(args, len);
  int n = run_test(gen_acceptable_kokushi, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, len, args);
  va_end(args);
  return n;
}

static int chiitoitsu(MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6, MJTileId t7,
                      MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12, MJTileId t13, int len, ...) {
  va_list args;
  va_start(args, len);
  int n = run_test(gen_acceptable_chiitoitsu, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, len, args);
  va_end(args);
  return n;
}

static int normal(MJTileId t1, MJTileId t2, MJTileId t3, MJTileId t4, MJTileId t5, MJTileId t6, MJTileId t7,
                  MJTileId t8, MJTileId t9, MJTileId t10, MJTileId t11, MJTileId t12, MJTileId t13, int len, ...) {
  va_list args;
  va_start(args, len);
  int n = run_test(gen_acceptable_normal, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, len, args);
  va_end(args);
  return n;
}

static void test_kokushi() {
  assert(kokushi(m1, m9, p1, p9, s1, s9, wt, wn, ws, wp, dw, dg, dr, 13, m1, m9, p1, p9, s1, s9, wt, wn, ws, wp, dw, dg,
                 dr) == 0);
  assert(kokushi(m1, m1, p1, p9, s1, s9, wt, wn, ws, wp, dw, dg, dr, 1, m9) == 0);
  assert(kokushi(m1, m1, p2, p9, s1, s9, wt, wn, ws, wp, dw, dg, dr, 2, m9, p1) == 0);
  assert(kokushi(m1, m1, p2, p9, s1, s9, wt, wn, ws, wp, dw, dg, s4, 3, m9, p1, dr) == 0);
}

static void test_chiitoitsu() {
  assert(chiitoitsu(m1, m1, p1, p9, s1, s9, wt, wn, ws, wp, dw, dg, dr, 11, p1, p9, s1, s9, wt, wn, ws, wp, dw, dg,
                    dr) == 0);
  assert(chiitoitsu(m1, m1, m2, m2, m3, m3, m4, m4, m4, m5, dw, dg, dr, 4, m5, dw, dg, dr) == 0);
  assert(chiitoitsu(m1, m1, m2, m2, m3, m3, m4, m4, m4, m4, dw, dw, dr, 1, dr) == 0);
}

static void test_normal() {
  assert(normal(m1, m1, m2, m2, m3, m3, p1, p1, p2, p2, p3, p3, dr, 1, dr) == 0);
  assert(normal(m1, m1, m2, m2, m3, m4, p1, p1, p2, p2, p3, p3, dr, 6, m1, m2, m3, m4, m5, dr) == 0);
  assert(normal(m2, m2, m4, m4, m5, m5, m6, m7, m7, m8, m8, m8, m9, 8, m2, m3, m4, m5, m6, m7, m8, m9) == 0);
  assert(normal(m1, m2, m3, m3, m3, m3, m4, wt, wt, wt, wn, wn, wn, 4, m1, m2, m4, m5) == 0);
}

bool test_ukeire() {
  test_kokushi();
  test_chiitoitsu();
  test_normal();
  return true;
}
