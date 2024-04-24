#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define BUFFSIZE 10LLU

int main(void) {
    const wchar_t lowercase_alpha  = L'\x3B1';
    char          mbstr[BUFFSIZE]  = { 0 }; // a buffer to copy the multibyte character

    size_t        status           = 0;
    mbstate_t     state            = { 0 }; // to store the state of multibyte translation
    const errno_t wcrtomb_s_status = wcrtomb_s(&status, mbstr, BUFFSIZE, lowercase_alpha, &state);

    // wcrtomb_s will return 0 upon success
    if (wcrtomb_s_status) {
        _putws(L"oooops! Not a valid multibyte character in this machine's locale!");
        return EXIT_FAILURE;
    }
    wprintf_s(L"wcrtomb_s returned %d\n", wcrtomb_s_status);

    // intentionally using %lld for an unsigned type since https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/wcrtomb-s?view=msvc-170
    // says it will be -1 if the function failed! WTF??
    wprintf_s(L"All good :), %lld bytes were used to store the translated multibyte character!\n", status);
    for (size_t i = 0; i < BUFFSIZE; ++i) putchar(mbstr[i]);

    int _status = 0;
    switch (wctomb_s(&_status, mbstr, BUFFSIZE, lowercase_alpha)) {
        case EINVAL : fputws(L"EINVAL\n", stderr); break;
        case EILSEQ : fputws(L"EILSEQ\n", stderr); break;
        default     : break;
    }

    wprintf_s(L"All good :), %d bytes were used to store the translated multibyte character!\n", _status);
    for (size_t i = 0; i < BUFFSIZE; ++i) putchar(mbstr[i]);

    return EXIT_SUCCESS;
}
