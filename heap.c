#include <heap.h>

static inline bool compare(_In_ const void* const restrict child, _In_ const void* const restrict parent) {
    return (*(unsigned*) child > *(unsigned*) parent) ? true : false;
}

int wmain(void) {
    heap_t heap = { 0 };
    heap_init(&heap, compare);

    unsigned* ptr_0 = malloc(sizeof(unsigned));
    unsigned* ptr_1 = malloc(sizeof(unsigned));
    assert(ptr_0);
    assert(ptr_1);
    *ptr_0              = 9;
    *ptr_1              = 19;

    unsigned* retrieved = NULL;

    heap_push(&heap, ptr_0);
    heap_push(&heap, ptr_1);

    heap_pop(&heap, &retrieved);
    wprintf_s(L"%u\n", *retrieved); // should print 19
    *retrieved = 0;

    heap_pop(&heap, &retrieved);
    wprintf_s(L"%u\n", *retrieved); // should print 9

    // heap_clean(&heap);

    return EXIT_SUCCESS;
}
