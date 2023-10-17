#pragma once

// header only implementation of a top heavy heap.

#ifndef __HEAP__
    #define __HEAP__
    #include <memory.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <stdlib.h>

typedef struct heap {
        size_t size;
        bool   (*fnptr_pred)(_In_reads_(1) const void* this, _In_reads_(1) const void* next); // predicate
        void   (*fnptr_dstr)(_In_ const void* mem);                                           // destroyer
        void** tree;
} heap_t;

static void inline init_heap(
    _In_ heap_t* const restrict heap,
    _In_ bool (*predicate)(_In_reads_(1) const void* this, _In_reads_(1) const void* next),
    _In_ void (*destroy)(_In_ const void* mem)
) {
    heap->size       = 0;
    heap->fnptr_pred = predicate;
    heap->fnptr_dstr = destroy;
    heap->tree       = NULL;
    return;
}

static void inline clean_heap(_In_ heap_t* const restrict heap) {
    if (heap->fnptr_dstr)
        for (size_t i = 0; i < heap->size; ++i) heap->fnptr_dstr(heap->tree[i]);
    free(heap->tree);
    memset(heap, 0, sizeof(heap_t));
    return;
}

#endif // !__HEAP__