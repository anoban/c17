#pragma once
#ifndef __HUFFMAN_H__
    #define __HUFFMAN_H__

// clang-format off
    #define _AMD64_
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_MEAN
    // UCRT headers, for some freaking reason, use a plain #if predicate for __STDC_WANT_SECURE_LIB__
    // not an #ifdef predicate, so we need to provide a valued definition for __STDC_WANT_SECURE_LIB__,
    // a plain #define results in compile time error
    #define __STDC_WANT_SECURE_LIB__ 1
    #define NOMINMAX     // it seems that only <Windows.h> has the internal header guards receptive to NOMINMAX
    // if we include system headers directly without relying on <windows.h> for transient includes, #define NOMINMAX offers no help! YIKES!

    #include <windef.h>
    #include <errhandlingapi.h>
    #include <fileapi.h>
    #include <handleapi.h>
    #include <sal.h>
// clang-format on

    #include <assert.h>
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>

// clang-format off
    #include <bitops.h>
// clang-format on

// static_assert(max(10, 0), "see max is reachable here!");
    #if defined(min) && defined(max)
        #undef min
        #undef max
    #endif

typedef struct _node { // represents a Huffman node.
        uint8_t  byte;
        uint32_t freq;
} node_t;

static_assert(sizeof(node_t) == 8);
static_assert(offsetof(node_t, byte) == 0);
// weird but static_assert(!offsetof(node_t, byte)); errs with MSVC, but works with gcc, clang & icx
static_assert(offsetof(node_t, freq) == 4);

typedef struct _code { // represents a Huffman code
        bool     is_used;
        uint8_t  length;
        uint16_t code;
} code_t;

static_assert(sizeof(code_t) == 4);
static_assert(offsetof(code_t, is_used) == 0);
static_assert(offsetof(code_t, length) == 1);
static_assert(offsetof(code_t, code) == 2);

typedef struct _heap {     // heap
        uint32_t count;    // number of nodes.
        uint32_t capacity; // number of nodes the heap can hold before requiring a reallocation.
        bool     (*fnptr_pred)(_In_ const void* const restrict, _In_ const void* const restrict);
        void**   tree; // a heap allocated array containing pointers to heap allocated nodes.
                       // use malloc to allocate the tree and the nodes.
} heap_t;

static_assert(sizeof(heap_t) == 24);
static_assert(offsetof(heap_t, count) == 0);
static_assert(offsetof(heap_t, capacity) == 4);
static_assert(offsetof(heap_t, fnptr_pred) == 8);
static_assert(offsetof(heap_t, tree) == 16);

typedef struct _pque { // priority que
        uint32_t count;
        uint32_t capacity;
        bool     (*fnptr_pred)(_In_ const void* const restrict, _In_ const void* const restrict);
        void**   tree;
} pque_t;

static_assert(sizeof(pque_t) == 24);
static_assert(offsetof(pque_t, count) == 0);
static_assert(offsetof(pque_t, capacity) == 4);
static_assert(offsetof(pque_t, fnptr_pred) == 8);
static_assert(offsetof(pque_t, tree) == 16);

static __forceinline uint32_t __stdcall get_parent(_In_ const uint32_t cpos) {
    return cpos ? (cpos - 1) / 2 : 0 /* deliberate truncating division. */;
}

static __forceinline uint32_t __stdcall get_leftchild(_In_ const uint32_t ppos) { return ppos * 2 + 1; }

static __forceinline uint32_t __stdcall get_rightchild(_In_ const uint32_t ppos) { return ppos * 2 + 2; }

#endif // !__HUFFMAN_H__
