// cl .\dllibrary.c /Wall /O2 /Ot /std:c17 /c gives us  dllibrary.obj
// link dllibrary.obj /NOENTRY /DLL /EXPORT:<> gives us dllibrary.dll, dllibrary.exp & dllibrary.lib
// <TU name>.lib is the export library produced by the linker, any programme that wishes to consume this DLL must link this export library

// Why DLLs?

// advantages ::
// many exes can share code with dynamic link libraries (DLLs)
// this results in lean exes compared to static linking.
// reduces disk space use
// reduces runtime overall memory use, read only pages of DLLs could be shared between processes.
// componentization of code
// improved serviceability (bug fixes, refactoring, security patches..) can be realized in the DLL itself, the consuming exe doesn't need to be
// updated in any war or form in contrast to having to recompile and re-link the exe with the static library.
// improved maintainability

// drawbacks ::
// DLLs have a performance overhead compared to static linking
// load time linking vs runtime linking
// the DLL needs to be ransacked with a strcmp like function to traverse the binary to find the needed function pointer.
// practically impossible to optimize code across DLL boundaries e.g LTCG, LTO, inlining etc won't work

// when code has to interoperate across a DLL boundary, it's better to explicitly specify the calling convention
long double __declspec(dllexport) __cdecl factorial(unsigned n) { // function exported from this TU
    long double result = 1.00L;
    if (!n) return 1.000L;
    for (unsigned i = n; i > 0; --i) result *= i;
    return result;
}

// using a different calling convention, without an explicit dllexport attribute
// if an explicit __declspec(dllexport) attribute is absent in the function signature, pass /EXPORT:<name> to the linker so it
// includes the given routine in the export library
unsigned long long __stdcall gcd(const unsigned long long a, const unsigned long long b) {
    for (unsigned long long min = a > b ? b : a; min > 0; --min)
        if (!(a % min) && !(b % min)) return min;
    return 1;
}

// structure of a binary DLL
// 1) DOS stub - from a time when Windows and DOS interoperated, completely useless now.
// 2) PE signature - PE\0\0
// 3) COFF file header
