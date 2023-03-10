/*
 *  MIT License
 *  
 *  Copyright (c) 2023 otamajakusi
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "meld.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mahjong.h"
#include "tile.h"

static bool is_valid_meld(const MJMeld *meld) {
  if (meld->len != 3 && meld->len != 4) {
    fprintf(stderr, "invalid meld len %d\n", meld->len);
    return false;
  }
  for (uint32_t i = 0; i < meld->len; i++) {
    if (!is_tile_id_valid(meld->tile_id[i])) {
      fprintf(stderr, "invalid meld tile %d\n", meld->tile_id[i]);
      return false;
    }
  }
  return true;
}

bool is_valid_melds(const MJMelds *melds) {
  if (melds->len > MJ_ELEMENTS_LEN) {
    return false;
  }
  for (uint32_t i = 0; i < melds->len; i++) {
    if (!is_valid_meld(&melds->meld[i])) {
      return false;
    }
  }
  return true;
}
