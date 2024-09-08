// TOWER OF HANOI

// RULES ::
// 1) ONLY ONE DISC CAN BE MOVED AT A TIME
// 2) ONLY THE UPPERMOST DISC ON A RACK CAN BE MOVED
// 3) AND A MOVED DISC CAN ONLY PLACED AT THE UPPERMOST POSITION OF A RACK
// 4) DISC PLACEMENT IS VALID ONLY IF THE CANDIDATE DISC HAS A SMALLER DIAMETER THAN THE DISC BELOW IT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_DISCS  50LLU // maximum discs allowed in a game
#define MAX_DIAMATER 100   // maximum diameter a disc can have

typedef struct board { // the game board with three towers
        int a[TOTAL_DISCS];
        int b[TOTAL_DISCS];
        int c[TOTAL_DISCS];
} board_t;

static __forceinline void __stdcall prepare_board(_In_ register board_t* const restrict object) {
    // decide the number of discs to place each of the three towers
    const unsigned discs_tower_0 = rand() + 1 % TOTAL_DISCS; // must be 100 or less (could not be 0)
    const unsigned discs_tower_1 = (TOTAL_DISCS - discs_tower_0) % ((rand() + 1) % (TOTAL_DISCS - discs_tower_0));
    const unsigned discs_tower_2 = TOTAL_DISCS - (discs_tower_0 + discs_tower_1);
    assert((discs_tower_0 + discs_tower_1 + discs_tower_2) == TOTAL_DISCS);

    unsigned i = 0;
    for (; i < discs_tower_0; ++i) object->a[i] = rand() % MAX_DIAMATER;
    for (i = 0; i < discs_tower_1; ++i) object->b[i] = rand() % MAX_DIAMATER;
    for (i = 0; i < discs_tower_2; ++i) object->c[i] = rand() % MAX_DIAMATER;

    _putws(L"initialization successfull!");
}

static __forceinline bool __stdcall check_towers(const board_t* const restrict board) { }

#pragma region TEST

static_assert(sizeof(board_t) == sizeof(int) * TOTAL_DISCS * 3);
static_assert(offsetof(board_t, a) == sizeof(int) * TOTAL_DISCS * 0);
static_assert(offsetof(board_t, b) == sizeof(int) * TOTAL_DISCS * 1);
static_assert(offsetof(board_t, c) == sizeof(int) * TOTAL_DISCS * 2);

static inline void test_init_towers(void) {
    board_t dummy = { 0 };
    for (unsigned i = 0; i < 200; ++i) prepare_board(&dummy);
}

#pragma endregion

int main(void) {
    srand(time(NULL)); // globally seed the random number generator
    static board_t hanoi = { 0 };

    test_init_towers();

    prepare_board(&hanoi);
    return EXIT_SUCCESS;
}
