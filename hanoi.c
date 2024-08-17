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

#define TOTAL_DISCS  100LLU // maximum discs allowed in a game
#define MAX_DIAMATER 100    // maximum diameter a disc can have

typedef struct tower {
        int tower_0[TOTAL_DISCS];
        int tower_1[TOTAL_DISCS];
        int tower_2[TOTAL_DISCS];
} tower_t;

static __forceinline void __stdcall init_towers(tower_t* const restrict object) {
    // decide the number of discs to place each of the three towers
    const unsigned discs_tower_0 = rand() % TOTAL_DISCS;
    const unsigned discs_tower_1 = (TOTAL_DISCS - discs_tower_0) % (rand() % (TOTAL_DISCS - discs_tower_0));
    const unsigned discs_tower_2 = TOTAL_DISCS - (discs_tower_0 + discs_tower_1);
    assert(discs_tower_0 + discs_tower_1 + discs_tower_2 == TOTAL_DISCS);

    unsigned i = 0;
    for (; i < discs_tower_0; ++i) object->tower_0[i] = rand() % MAX_DIAMATER;
    for (i = 0; i < discs_tower_1; ++i) object->tower_1[i] = rand() % MAX_DIAMATER;
    for (i = 0; i < discs_tower_2; ++i) object->tower_2[i] = rand() % MAX_DIAMATER;

    _putws(L"initialization successfull!");
}

static __forceinline bool __stdcall check_towers(const tower_t* const restrict tower) { }

#pragma region TEST

static_assert(sizeof(tower_t) == 1200LLU);
static_assert(offsetof(tower_t, tower_0) == 0LLU);
static_assert(offsetof(tower_t, tower_1) == 400LLU);
static_assert(offsetof(tower_t, tower_2) == 800LLU);

static inline void test_init_towers() {
    tower_t dummy = { 0 };
    for (unsigned i = 0; i < 200; ++i) init_towers(&dummy);
}

#pragma endregion

int main(void) {
    srand(time(NULL)); // globally seed the random number generator
    static tower_t hanoi = { 0 };

    test_init_towers();

    init_towers(&hanoi);
    return EXIT_SUCCESS;
}
