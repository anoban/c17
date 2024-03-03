#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    const int32_t     x = 86;
    const long double y = 3.534;
    switch (x) {
        case 86 : _putws(L"86"); // fallthrough
        case 85 : _putws(L"85"); // fallthrough
        case 84 : _putws(L"84"); // fallthrough
        case 83 : _putws(L"83"); break;
        case 82 : _putws(L"82"); // fallthrough
        default : _putws(L"default"); break;
    }

    // switch (y) { // switch accepts only integer types.
    //     case /* constant-expression */ :
    //         /* code */
    //         break;
    //
    //     default : break;
    // }

    switch (84) {
        case 86 : _putws(L"86");     // fallthrough
        case 85 : _putws(L"85");     // fallthrough
        case 84 : _putws(L"84");     // fallthrough
        case 83 : _putws(L"83"); break;
        case 82 : _putws(L"82");     // fallthrough
        default : _putws(L"default"); break;
    }

    switch (80 + 2) {
        case 86     : _putws(L"86"); // fallthrough
        case 85     : _putws(L"85"); // fallthrough
        case 84     : _putws(L"84"); // fallthrough
        case 83     : _putws(L"83"); break;
        // case contexts need to be integer contant expressions.
        case 80 + 2 : _putws(L"82"); // fallthrough
        default     : _putws(L"default"); break;
    }

    int z = x + abs(powl(x, M_PI));
    switch (x) {
        case 86 : _putws(L"86");     // fallthrough
        case 85 : _putws(L"85");     // fallthrough
        case 84 : _putws(L"84");     // fallthrough
        case 83 : _putws(L"83"); break;
        // z is not an integer constant expression (ICE).
        case z  : _putws(L"82"); // fallthrough
        default : _putws(L"default"); break;
    }
    return 0;
}
