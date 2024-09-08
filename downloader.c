#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "./httplib.h"

int wmain(int argc, wchar_t* argv[]) {
    WCHAR wstrServer[CHARBUFFSIZE] = { 0 }, wstrAccess[CHARBUFFSIZE] = { 0 }, wstrFileName[CHARBUFFSIZE] = { 0 };

    for (int i = 1; i < argc; ++i) {
        UrlSplitterW(argv[i], wstrServer, wstrAccess, wstrFileName, wcslen(argv[i]));

        SCRHANDLES scrHandles = HttpGet(wstrServer, wstrAccess);

        DWORD64 dwNBytes      = 0;
        LPSTR   pszBytes      = ReadHttpResponse(scrHandles, &dwNBytes);

        if (SaveBufferToDisk(pszBytes, dwNBytes, wstrFileName)) {
            // Once SaveBufferToDisk() is called with a bytes stream, the buffer will get freed
            // and won't exist in the heap.
            // pszBytes will be NULL.

            printf_s("Download was successfull.\n");
            return EXIT_SUCCESS;
        } else {
            fprintf_s(stderr, "Error %lu in saving file.\n", GetLastError());
            return EXIT_FAILURE;
        }

        memset(wstrServer, 0, CHARBUFFSIZE);
        memset(wstrAccess, 0, CHARBUFFSIZE);
        memset(wstrFileName, 0, CHARBUFFSIZE);
    }

    return 0;
}