#include <huffman.h>

#define DEFAULT_HEAP_CAPACITY       1024LLU
#define DEFAULT_HEAP_CAPACITY_BYTES (DEFAULT_HEAP_CAPACITY * sizeof(uintptr_t))

// many problems demand determination of the smallest or largest element from a collection that's capable of frequent insertions and deletions
// one way to meet this need is to keep the collection sorted all the time but sorting a collection at every insertion or deletion is expensive

// fortunately this requirement does not insinuate that all the elements in a collection need to be sorted.
// it's enough for the collection to just know the location of the smallest or the largest element, enabling fast access.
// heaps and priority ques are adept for this purpose

// heaps are organized trees (usually a binary tree) that allow easy access to the largest / smallest element.
// the cost of creating and maintaining a tree is negligible compared to the performance overhead incurred by maintaining the container sorted all the time

// in a heap, each child node has a weight smaller than its parent's, ergo it is the root node that has the largest weight in a heap
// binary trees like these are top heavy, as we climb up the hierarchy of nodes, nodes will become more and more heavy (will have a larger weight)

// trees can also be architectured to hold the heaviest nodes at the bottom and lightest nodes at the top, whereby the root node will be the one with the smallest weight
// in that case, parent node will have a smaller weight compared to its children, hence making the tree bottom heavy

// heaps are typically left balanced.
// when new nodes are annexed to the tree at a given level, the tree grows from left to right.

// an efficient way to implement left-balanced binary tree is to store the nodes in a contiguous array.
// nodes are arranged in this array in the order we'd encounter them in a level traversal.

// say that we have 11 leaf nodes with weights 15, 7, 9, 18, 10, 12, 17, 19, 20, 22 ans 25 to begin with.
// NOTE: parent nodes here aren't created from the children!
// THIS TREE ISN'T REPRESENTATIVE OF AN ORDERED HEAP. JUST AN EXEMPLAR FOR REPRESENTING A TREE USING AN ARRAY.

/*
                            (25)             <--- 3rd (Root/Top)
                           /    \
                          /      \
                         /        \
                        /          \
                      (20)         (22)      <--- 2nd
                     /    \        /  \
                    /      \      /    \
                  (17)     (19) (10)  (12)   <--- 1st
                  /  \     /  \
                 /    \   /    \
               (15)   (7)(9)  (18)           <--- 0th (Bottom)
*/

// the array representation will be:
// we move left -> right, through the hierarchies.

/*
 {25}:                                          (after traversing the 3rd level)
 {25, 20, 22}:                                  (after traversing the 2nd level)
 {25, 20, 22, 17, 19, 10, 12}:                  (after traversing the 1st level)
 {25, 20, 22, 17, 19, 10, 12, 15, 07, 09, 18}:  (after traversing the 0th level)
*/

// this design offers an inherent benefit that the parent node of a given node can easily be located by the
// offset (i - 1) / 2 where i is the offset of the chosen child node.

// E.g: what's the offset of node 19's parent?
// offset of node 19 is 4 (0 indexed array): i = 4
// (4 - 1) / 2 = 1 (integer division)
// node at offset 1 is 20, and it indeed is the parent of node 19.

// and the offset of a given node with offset i's left child is 2i + 1 and its right child is 2i + 2
// E.g. Take the node 20 for example, offset of node 20 is 1; i = 1
// left child = 2 * 1 + 1 = 3 and right child = 2 * 1 + 2 = 4
// 17 and 19 => correct!

// leftmost node must be the heaviest, and we do not care about the ordering of other nodes in the heap (array)

/*
typedef struct _heap {
         uint64_t count;    // number of nodes.
         uint64_t capacity; // number of nodes the heap can hold before requiring a reallocation.
         bool (*fnptr_pred)(_In_reads_(1) const void* const restrict child, _In_reads_(1) const void* const restrict parent);
         void (*fnptr_clean)(_In_reads_(1) const void* const restrict memblock);
         void** tree; // a heap allocated array containing pointers to heap allocated nodes.
                      // use malloc to allocate the tree and the nodes.
 } heap_t;
 */

/*
// must return true whenever a swap is needed.
static inline bool __cdecl predicate(const void* const restrict child, const void* const restrict parent){
    retrun child->comparable > parent->comparable ? true : false;
}
*/

static inline bool heap_init(
    _Inout_ heap_t* const restrict heap, _In_ bool (*const predicate)(_In_ const void* const restrict, _In_ const void* const restrict)
) {
    heap->count      = 0;
    heap->capacity   = DEFAULT_HEAP_CAPACITY; // this is the number of pointers, NOT BYTES
    heap->fnptr_pred = predicate;
    heap->tree = malloc(DEFAULT_HEAP_CAPACITY_BYTES); // will allocate memory to store DEFAULT_HEAP_CAPACITY number of pointers in the tree.

    if (!heap->tree) {
        fwprintf_s(stderr, L"memory allocation error inside %s @LINE: %d\n", __FUNCTIONW__, __LINE__);
        return false;
    }

    memset(heap->tree, 0U, DEFAULT_HEAP_CAPACITY_BYTES); // necessary??
    return true;
}

static inline bool heap_push(
    _Inout_ heap_t* const restrict heap,
    _In_ const void* const restrict data /* expects a heap allocated memory block to push into the heap */
) {
    void*  _temp     = NULL;
    size_t _childpos = 0, _parentpos = 0; //

    if (heap->count + 1 > heap->capacity) { // if the existing buffer doesn't have space for another pointer,
        // ask for an additional DEFAULT_HEAP_CAPACITY_BYTES bytes. return false if the reallocation failed.
        if (!(_temp = realloc(heap->tree, (heap->capacity + DEFAULT_HEAP_CAPACITY) * sizeof(uintptr_t) /* new size */))) {
            // heap->tree is still valid and points to the old buffer
            fwprintf_s(stderr, L"memory reallocation error inside %s @LINE: %d\n", __FUNCTIONW__, __LINE__);
            return false;
        }

        heap->tree     = _temp;                                  // if reallocation was successful, reassign the new memory block to tree.
        heap->capacity = heap->capacity + DEFAULT_HEAP_CAPACITY; // update the capacity
    }

    // consider our previous tree:
    // { 25, 20, 22, 17, 19, 10, 12, 15, 07, 09, 18 }
    // let's try and push a node with weight 24
    // after pushing we'd end up with an array like this, with the pushed node appended at the rightmost end.
    // { 25, 20, 22, 17, 19, 10, 12, 15, 07, 09, 18, 24 }
    // 24 is at offset 11, that makes it a child of node (i - 1) / 2 = 5, that is node 10.

    /* now the tree assumes a shape like this:

                               (25)               <--- 3rd (Root/Top)
                             /     \
                            /       \
                           /         \
                          /           \
                         /             \
                        /               \
                      (20)              (22)      <--- 2nd
                     /    \             /  \
                    /      \           /    \
                  (17)     (19)      (10)  (12)   <--- 1st
                  /  \     /  \      /
                 /    \   /    \    /
               (15)   (7)(9)  (18)(24)            <--- 0th (Bottom)
    */

    // since a child node cannot have a bigger weight than its parent node in a top-heavy heap, this needs reordering.
    // to make this push comply with the heap requisites, we'll swap child and parents until we get to an acceptable structure.

    // first swap is between 24 and its parent 10;
    /* now the tree assumes a shape like this:

                               (25)               <--- 3rd (Root/Top)
                             /     \
                            /       \
                           /         \
                          /           \
                         /             \
                        /               \
                      (20)              (22)      <--- 2nd
                     /    \             /  \
                    /      \           /    \
                  (17)     (19)      (24)  (12)   <--- 1st
                  /  \     /  \      /
                 /    \   /    \    /
               (15)   (7)(9)  (18)(10)            <--- 0th (Bottom)
    */

    // still we have 22 as parent of 24, swap these two again;
    /* now the tree assumes a shape like this:

                               (25)                <--- 3rd (Root/Top)
                             /     \
                            /       \
                           /         \
                          /           \
                         /             \
                        /               \
                      (20)              (24)       <--- 2nd
                     /    \             /  \
                    /      \           /    \
                  (17)     (19)      (22)  (12)    <--- 1st
                  /  \     /  \      /
                 /    \   /    \    /
               (15)   (7)(9)  (18)(10)             <--- 0th (Bottom)
    */
    // {25, 20, 24, 17, 19, 22, 12, 15, 7, 9, 18, 10}
    // perfecto :)))

    heap->count++;                                       // increment the node count.
    heap->tree[heap->count - 1] = data;                  // hook in the new node
    _childpos                   = heap->count - 1;       // offset of the newly inserted node.
    _parentpos                  = get_parent(_childpos); // offset of the new node's parent.

    while ((_childpos > 0) /* as long as we haven't reached the root node at offset 0 */ &&
           heap->fnptr_pred(
               heap->tree[_childpos], heap->tree[_parentpos]
           ) /* and the weight of the child is greater than that of the parent */) {
        // two conditions needed to perform the swap

        _temp                  = heap->tree[_childpos];
        heap->tree[_parentpos] = heap->tree[_childpos];
        heap->tree[_childpos]  = _temp;
        _temp                  = NULL;

        // prepare for the next swap in anticipation that the new positioning of the inserted node also needs a swap.
        _childpos              = _parentpos;            // previous parent's offset now becomes the child's offset.
        _parentpos             = get_parent(_childpos); // find the parent of the current child's offset.
    }

    return true;
}

static inline void heap_clean(_Inout_ heap_t* const restrict heap) {
    for (size_t i = 0; i < heap->count; ++i) free(heap->tree[i]); // free the heap allocated nodes.
    free(heap->tree);                                             // free the array containing pointers to heap allocated nodes.
    memset(heap, 0U, sizeof(heap_t));                             // zero out the struct
}

static inline bool heap_pop(_Inout_ heap_t* const restrict heap, _Inout_ void** restrict data /* popped out pointer */) {
    size_t leftchildpos = 0, rightchildpos = 0, parentpos = 0, pos = 0;
    void*  tmp = NULL;

    // if the heap is empty,
    if (!heap->count) {
        *data = NULL;
        return false;
    }

    // if the heap has only one node,
    if (heap->count == 1) {
        *data = heap->tree[0];
        heap->count--;
        heap_clean(heap); // since heap->count = 0, the looped free()s inside heap_clean() won't be executed.
        return true;
    }

    // {25, 20, 24, 17, 19, 22, 12, 15, 7, 9, 18, 10} ----------\
                                                             // |
    // give up the node at the top (root node)               // |
    *data                       = heap->tree[0]; // |
    heap->tree[0]               = NULL;          // |
                                                 // |
    // {NULL, 20, 24, 17, 19, 22, 12, 15, 7, 9, 18, 10} <-------/

    /* now the tree looks like this:

                             (NULL)                <--- 3rd (Root/Top)
                             /     \
                            /       \
                           /         \
                          /           \
                         /             \
                        /               \
                      (20)              (24)       <--- 2nd
                     /    \             /  \
                    /      \           /    \
                  (17)     (19)      (22)  (12)    <--- 1st
                  /  \     /  \      /
                 /    \   /    \    /
               (15)   (7)(9)  (18)(10)             <--- 0th (Bottom)
    */

    // move the last node in the array to the root's position, offset 0.
    heap->tree[0]               = heap->tree[heap->count - 1];
    heap->tree[heap->count - 1] = NULL;
    heap->count--;

    /* now the tree looks like this:

                               (10)                <--- 3rd (Root/Top)
                             /     \
                            /       \
                           /         \
                          /           \
                         /             \
                        /               \
                      (20)              (24)       <--- 2nd
                     /    \             /  \
                    /      \           /    \
                  (17)     (19)      (22)  (12)    <--- 1st
                  /  \     /  \
                 /    \   /    \
               (15)   (7)(9)  (18)                 <--- 0th (Bottom)
    */
    // {10, 20, 24, 17, 19, 22, 12, 15, 7, 9, 18}
    // rearrange the tree

    while (true) {
        // at the onset of iteration, parentpos = 0; currently points to the (new) root node.
        leftchildpos  = get_leftchild(parentpos);
        rightchildpos = get_rightchild(parentpos);

        if (leftchildpos < heap->count /* until we reach the rightmost end */ &&
            heap->fnptr_pred(
                heap->tree[leftchildpos], heap->tree[parentpos]
            )) { // if the weight of the left child is greater than that of the parent,
            // choose to traverse down the left arm.
            pos = leftchildpos;
        } else {
            pos = parentpos; // otherwise hold the caret at the parent.
        }
        // in our example, this conditional will choose the left arm.

        if (rightchildpos < heap->count /* until we reach the rightmost end */ &&
            heap->fnptr_pred(
                heap->tree[rightchildpos], heap->tree[pos]
            )) { // if the weight of the right child is greater than that of the parent OR the left child (CONTEXT DEPENDENT)
            // choose to traverse down the right arm
            pos = rightchildpos;
            // in our example this conditional will choose the right arm, because 24 > 10.
        }

        // in our example, both conditionals at line 310 and 318 will evaluate to true.
        // since the expressions predicted on the conditional at line 318 will be evaluated finally,
        // it is the right child that will be swapped with the parent first.

        // if the root is heavier than both the left and right children, no need to rearrange the heap.
        if (pos == parentpos) break;

        tmp                      = heap->tree[parentpos];
        heap->tree[parentpos]    = heap->tree[leftchildpos];
        heap->tree[leftchildpos] = tmp;
        tmp                      = NULL;

        /* now the tree looks like this:

                          (24)                <--- 3rd (Root/Top)
                        /     \
                       /       \
                      /         \
                     /           \
                    /             \
                   /               \
                 (20)              (10)       <--- 2nd
                /    \             /  \
               /      \           /    \
             (17)     (19)      (22)  (12)    <--- 1st
             /  \     /  \
            /    \   /    \
          (15)   (7)(9)  (18)                 <--- 0th (Bottom)
       */

        // move down one level. i.e the current right child becomes the parent node for the next iteration.
        parentpos                = pos;
    }

    /* at the start of second iteration, the tree looks like this:

                           (24)                <--- 3rd (Root/Top)
                         /     \
                        /       \
                       /         \
                      /           \
                     /             \
                    /               \
                  (20)              (10)       <--- 2nd
                 /    \             /  \
                /      \           /    \
              (17)     (19)      (22)  (12)    <--- 1st
              /  \     /  \
             /    \   /    \
           (15)   (7)(9)  (18)                 <--- 0th (Bottom)
    */
    // again the first conditional will pick the left arm for swapping as 22 > 10.
    // second conditional won't execute as 22 < 12, finally the left arm will be selected for swapping.

    /* at the end of second iteration, the tree looks like this:

                           (24)                <--- 3rd (Root/Top)
                         /     \
                        /       \
                       /         \
                      /           \
                     /             \
                    /               \
                  (20)              (22)       <--- 2nd
                 /    \             /  \
                /      \           /    \
              (17)     (19)      (12)  (10)    <--- 1st
              /  \     /  \
             /    \   /    \
           (15)   (7)(9)  (18)                 <--- 0th (Bottom)
     */
    // no rearrangements needed anymore.

    return true;
}
