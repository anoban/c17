#pragma once
#ifndef __MASTER__
    #define __MASTER__

// a header that includes all the C standard headers and Windows headers.
// to be included in user defined headers.

    #include <assert.h>
    #include <math.h>
    #include <stdalign.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_MEAN
    #include <Windows.h>

#endif // __MASTER__
