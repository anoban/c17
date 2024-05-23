#include <assert.h>
#include <httplib.h>
#include <stdio.h>

BOOL ActivateVirtualTerminalEscapes(VOID) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        fprintf_s(stderr, "Error %ld in getting hConsole handle.\n", GetLastError());
        return FALSE;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hConsole, &dwMode)) {
        fprintf_s(stderr, "Error %ld in getting console mode.\n", GetLastError());
        return FALSE;
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hConsole, dwMode)) {
        fprintf_s(stderr, "Error %ld in enabling virtual terminal escapes.\n", GetLastError());
        return FALSE;
    }
    return TRUE;
}

SCRHANDLES HttpGet(LPCWSTR pswzServerName, LPCWSTR pswzAccessPoint) {
    /*
     * A convenient wrapper around WinHttp functions.
     * Allows to send a GET request and receive the response in one function call
     * without having to deal with the cascade of WinHttp callbacks.
     */

    HINTERNET  hSession = NULL, hConnection = NULL, hRequest = NULL;
    BOOL       bHttpResults = FALSE;
    SCRHANDLES scrStruct    = { .hSession = NULL, .hConnection = NULL, .hRequest = NULL };

    // Returns a valid session handle if successful, or NULL otherwise.
    // first of the WinHTTP functions called by an application.
    // It initializes internal WinHTTP data structures and prepares for future calls from the application.
    hSession                = WinHttpOpen(
        // impersonating Firefox to avoid request denials from automated clients by servers.
        L"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:126.0) Gecko/20100101 Firefox/126.0",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    );

    if (hSession) {
        // Specifies the initial target server of an HTTP request and returns an HINTERNET connection handle
        // to an HTTP session for that initial target.
        // Returns a valid connection handle to the HTTP session if the connection is successful, or NULL otherwise.
        hConnection = WinHttpConnect(
            hSession,
            pswzServerName,
            INTERNET_DEFAULT_HTTP_PORT, // Uses port 80 for HTTP and port 443 for HTTPS.
            0
        );
    }

    if (hConnection) {
        // Creates an HTTP request handle.
        // An HTTP request handle holds a request to send to an HTTP server and contains all
        // RFC822/MIME/HTTP headers to be sent as part of the request.
        LPCWSTR ppwszMediaTypes[] = { L"mp4", L"json", L"mpeg4-generic", 0U };

        hRequest                  = WinHttpOpenRequest(
            hConnection,
            L"GET",
            pswzAccessPoint,
            NULL, // Pointer to a string that contains the HTTP version. If this parameter is NULL, the function uses HTTP/1.1
            WINHTTP_NO_REFERER,
            ppwszMediaTypes, // Pointer to a null-terminated array of string pointers that
            // specifies media types accepted by the client.
            // WINHTTP_DEFAULT_ACCEPT_TYPES, no types are accepted by the client.
            // Typically, servers handle a lack of accepted types as indication that the client accepts
            // only documents of type "text/*"; that is, only text documents & no pictures or other binary files
            0
        );
    }

    if (hRequest) {
        // Sends the specified request to the HTTP server.
        // Returns TRUE if successful, or FALSE otherwise.
        bHttpResults = WinHttpSendRequest(
            hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS, // A pointer to a string that contains the additional headers to append to the request.
            0,                       // An unsigned long integer value that contains the length, in characters, of the additional headers.
            WINHTTP_NO_REQUEST_DATA, // A pointer to a buffer that contains any optional data to send immediately after the request headers
            0,                       // An unsigned long integer value that contains the length, in bytes, of the optional data.
            0,                       // An unsigned long integer value that contains the length, in bytes, of the total data sent.
            0
        );  // A pointer to a pointer-sized variable that contains an application-defined value that is passed, with the request handle, to
            // any callback functions.
    }

    if (!bHttpResults) {
        fprintf_s(stderr, "Error %ld in the HttpGet procedure.\n", GetLastError());
        return scrStruct;
    }

    // these 3 handles need to be closed by the caller.
    scrStruct.hSession    = hSession;
    scrStruct.hConnection = hConnection;
    scrStruct.hRequest    = hRequest;

    return scrStruct;
}

LPSTR ReadHttpResponse(SCRHANDLES scrHandles, DWORD64* dwnReceivedBytes) {
    // if the call to HttpGet() failed,
    if (scrHandles.hSession == NULL || scrHandles.hConnection == NULL || scrHandles.hRequest == NULL) {
        fprintf_s(stderr, "ReadHttpResponse failed. Possible errors in previous call to HttpGet.\n");
        return NULL;
    }

    // this procedure is required to close these three handles.
    HINTERNET hSession = scrHandles.hSession, hConnection = scrHandles.hConnection, hRequest = scrHandles.hRequest;

    // Calling malloc first and then calling realloc in a do while loop is terribly inefficient for a
    // simple app sending a single GET request.
    // So, malloc all the needed memory beforehand and use a moving pointer to keep track of the
    // last write offset, so the next write operation can start from there such that we can prevent
    // overwriting previously written memory.

    LPSTR pszBuffer = (LPSTR) malloc(RESP_BUFF_SIZE); // now that's 1 GiB.
    // if malloc() failed,
    if (!pszBuffer) {
        fprintf_s(stderr, "Memory allocation failed. Error %ld\n", GetLastError());
        return NULL;
    }

    ZeroMemory(pszBuffer, RESP_BUFF_SIZE); // zero out the buffer.
    LPSTR pszLastWriteOffset = pszBuffer;

    BOOL bReception          = FALSE;
    bReception               = WinHttpReceiveResponse(hRequest, NULL);

    if (!bReception) {
        fprintf_s(stderr, "Failed to read the HTTP response. Error %ld in WinHttpReceiveResponse\n", GetLastError());
        return NULL;
    }

    DWORD64 dw64TotalBytesInResponse = 0, dw64TotalBytesReadFromResponse = 0;
    DWORD   dwBytesInCurrentQuery = 0, dwReadBytesInCurrentQuery = 0;

    do {
        dwBytesInCurrentQuery     = 0;
        dwReadBytesInCurrentQuery = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &dwBytesInCurrentQuery)) {
            fprintf_s(stderr, "Failed to query the response. Error %ld in WinHttpQueryDataAvailable\n", GetLastError());
            break;
        }

        // If there aren't any more bytes to read,
        if (!dwBytesInCurrentQuery) break;

        if (!WinHttpReadData(hRequest, pszLastWriteOffset, dwBytesInCurrentQuery, &dwReadBytesInCurrentQuery)) {
            fprintf_s(stderr, "Failed to read bytes from the response. Error %ld in WinHttpReadData\n", GetLastError());
            break;
        }

        // Increment the total counters.
        dw64TotalBytesInResponse       += dwBytesInCurrentQuery;
        dw64TotalBytesReadFromResponse += dwReadBytesInCurrentQuery;

        // Move the caret for next write.
        pszLastWriteOffset             += dwReadBytesInCurrentQuery;

    } while (dwBytesInCurrentQuery > 0);

    // Using the base CloseHandle() here will (did) crash the debug session.
    WinHttpCloseHandle(hSession);
    WinHttpCloseHandle(hConnection);
    WinHttpCloseHandle(hRequest);

    *dwnReceivedBytes = dw64TotalBytesReadFromResponse;
    printf_s("%.5Lf MiBs have been received in total.\n", ((long double) dw64TotalBytesReadFromResponse) / 1048576U);

    return pszBuffer;
}

BOOL SaveBufferToDisk(LPSTR pszBuffer, DWORD64 dwBuffSize, LPCWSTR pswzFileName) {
    /*
     * Takes in a byte stream to be write to the disk.
     * Second argument specifies the number of bytes to read from the byte stream.
     * Third argument specifies the name of the file to write on disk.
     * File name must be postfixed with an appropriate extension.
     * Once the buffer is flushed to the disk, frees the buffer.
     */

    HANDLE hFile = CreateFileW(pswzFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf_s(stderr, "Error %lu in opening file.\n", GetLastError());
        return FALSE;
    }

    DWORD dwnBytesWritten = 0;

    if (!WriteFile(hFile, pszBuffer, dwBuffSize, &dwnBytesWritten, NULL)) {
        fprintf_s(stderr, "Error %lu in writing to file.\n", GetLastError());
        return FALSE;
    }

#ifdef _DEBUG
    printf_s("Written %.5Lf MiBs to disk.\n", ((long double) dwnBytesWritten / 1048576U));
#endif // _DEBUG

    if (!CloseHandle(hFile)) {
        fprintf_s(stderr, "Error %ul in closing the file handle.\n", GetLastError());
        return FALSE;
    }

    free(pszBuffer); // free the buffer.
    pszBuffer = NULL;
    return TRUE;
}

void __stdcall UrlSplitterW(
    LPCWSTR _In_ pswzSrc, LPWSTR _Out_ pswzServerName, LPWSTR _Out_ pswzAccessPoint, LPWSTR _Out_ pswzFileName, DWORD64 _In_ dwSrcLen
) {
    assert(pswzSrc);
    assert(pswzServerName);
    assert(pswzAccessPoint);

    BOOL  PAST_FSLASH = FALSE;
    DWORD dwSer = 0, dwAcc = 0, dwFname = 0, i = 0;

    while (pswzSrc[i]) {
        if (!PAST_FSLASH && (pswzSrc[i] != L'/')) {
            pswzServerName[dwSer] = pswzSrc[i];
            dwSer++;
            i++;
        } else {
            PAST_FSLASH            = TRUE;
            pswzAccessPoint[dwAcc] = pswzSrc[i];
            dwAcc++;
            i++;
        }
    }

    for (DWORD i = dwSrcLen; i > 0; --i) {
        if (pswzSrc[i] == L'/') {
            dwFname = i + 1;
            break;
        }
    }

    wcscpy_s(pswzFileName, CHARBUFFSIZE, pswzSrc + dwFname);

#ifdef _DEBUG
    wprintf_s(L"Server name: %ls\n", pswzServerName);
    wprintf_s(L"Access point name: %ls\n", pswzAccessPoint);
    wprintf_s(L"File name: %ls\n", pswzFileName);
#endif
}
