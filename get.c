// clang .\get.c .\httplib.c  -Wall -Wextra -pedantic -O3 -std=c23 -I./

#include <httplib.h>
#include <sal.h>
#include <stdio.h>
#include <stdlib.h>

// not bothering error handling
int wmain(void) {
    // https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfileex
    // const SCRHANDLES handles = HttpGet(L"learn.microsoft.com", L"/en-us/windows/win32/api/fileapi/nf-fileapi-readfileex");

    const SCRHANDLES handles  = HttpGet(L"python.org", L"/downloads/windows/");
    DWORD64          nbytes   = 0;
    const char*      response = ReadHttpResponse(handles, &nbytes);
    puts(response);
    free(response);
    return EXIT_SUCCESS;
}
