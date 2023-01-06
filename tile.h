#define OK                  0
#define ERR_ILLEGAL_PARAM   -1
#define ERR_NUM_TILES_SHORT -2
#define ERR_NUM_TILES_LARGE -3

#include <stdint.h>
#include <stdbool.h>

#define MAX_HAND_LEN    (4 * 4 + 2) // 4-kans and 1-head
#define MAX_MELD_LEN    4
#define MAX_YAKU_LEN    64
#define MAX_TILE_ID     43

typedef struct {
    uint8_t han;
    uint8_t fu;
    char yaku[MAX_YAKU_LEN];
} Score;

// chi, pon, kan(open and closed)
typedef struct {
    uint8_t tile[4]; // tile_id value should be specified.
    uint32_t len; // tile len. for chi or pon, 3 should be set, for kan, 4 should be set.
    uint8_t is_closed; // set true if closed-kan
} Meld;

typedef struct {
    Meld meld[MAX_MELD_LEN];
    uint32_t len;
} Melds;

typedef struct {
    uint8_t tile[MAX_HAND_LEN]; // tile_id value should be specified
    uint32_t len; // tile length
} Hands;

// return
//   0: success
//   -1: illegal params
// params
//   [out]
//     score: calculated Score
//   [in]
//     hands: all tiles include melds and win_tile
//     melds: list of meld
//     win_tile: win_tile
int32_t tile_get_score(Score *score, const Hands *hands, const Melds *melds, uint8_t win_tile, uint32_t is_ron);
