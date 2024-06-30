#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct node {
        unsigned char byte;
        unsigned      frequency;
} node_t;

// static_assert(!offsetof(node_t, byte));
static_assert(!false);
static_assert(!0);
assert(!false);

int wmain(void) { return EXIT_SUCCESS; }
