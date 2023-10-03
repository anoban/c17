#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_MEAN
#endif // _WIN32

#include <stdlib.h>
#include <Windows.h>
#include <Winhttp.h>

#define _DEBUG
#define CHARBUFFSIZE 250U

#pragma comment(lib, "Winhttp")

typedef struct {
        HINTERNET hSession;
        HINTERNET hConnection;
        HINTERNET hRequest;
} SCRHANDLES;

#define RESP_BUFF_SIZE 524288000U // 0.5 GiBs

// Prototypes.

BOOL       ActivateVirtualTerminalEscapes(VOID);
SCRHANDLES HttpGet(LPCWSTR pswzServerName, LPCWSTR pswzAccessPoint);
LPSTR      ReadHttpResponse(SCRHANDLES scrHandles, DWORD64* dwnReceivedBytes);
BOOL       SaveBufferToDisk(LPSTR pszBuffer, DWORD64 dwBuffSize, LPCWSTR pswzFileName);
void __stdcall UrlSplitterW(
    LPCWSTR _In_ pswzSrc, LPWSTR _Out_ pswzServerName, LPWSTR _Out_ pswzAccessPoint, LPWSTR _Out_ pswzFileName, DWORD64 _In_ dwSrcLen
);
