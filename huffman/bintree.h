#pragma once

#ifndef __BINTREE__
    #define __BINTREE__
    #include <stdbool.h>
    #include <stdint.h>
    #include <stdlib.h>

typedef struct BinTreeNode_ {
        void*                data;
        struct BinTreeNode_* left;
        struct BinTreeNode_* right;
} BinTreeNode;

typedef struct BinTree_ {
        int32_t      size;
        int32_t      (*compare)(_In_ const void* restrict key_1, _In_ const void* restrict key_2);
        void         (*destroy)(_In_ const void* data);
        BinTreeNode* root;
} BinTree;

static __forceinline int32_t      bt_getsize(_In_ const BinTree* const bintree) { return bintree->size; }
static __forceinline BinTreeNode* bt_getroot(_In_ const BinTree* const bintree) { return bintree->root; }
static __forceinline bool         is_leaf(_In_ const BinTreeNode* const node) { return node->left == NULL && node->right == NULL; }

#endif // !__BINTREE__