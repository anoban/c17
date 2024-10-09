#include <assert.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NELEMENTS 2048LLU

static inline int __cdecl ptrcompare( // to be used with qsort_s()
    _In_opt_ [[maybe_unused]] void* const restrict context,
    _In_ const void* const restrict current,
    _In_ const void* const restrict next
) {
    assert((uintptr_t) current & (uintptr_t) next);
    return (*(long double*) current == *(long double*) next) ? 0 :
           (*(long double*) current > *(long double*) next)  ? 1 :
                                                               -1; // NOLINT(readability-avoid-nested-conditional-operator)
}

int wmain(void) {
    srand((unsigned) time(NULL));
    long double array[NELEMENTS] = { 0 };
    for (unsigned i = 0; i < NELEMENTS; ++i) array[i] = rand() / 32767.0L;

    qsort_s(array, NELEMENTS, sizeof(long double*), ptrcompare, NULL);
    for (unsigned i = 0; i < NELEMENTS; ++i) wprintf_s(L"%Lf\n", array[i]);

    return EXIT_SUCCESS;
}
