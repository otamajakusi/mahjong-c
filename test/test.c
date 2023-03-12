#include "test.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool test() {
  test_tile();
  test_meld();
  test_hand();
  test_element();
  test_score();
  test_agari();
  test_mahjong();
  return true;
}

int main() {
  printf("mahjong\n");
  test();
  return 0;
}
