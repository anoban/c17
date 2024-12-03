#include <assert.h>
#include <fileapi.h>
#include <stdbool.h>
#include <stdint.h>
#include <Windows.h>

uint64_t __forceinline __stdcall FNV1aHash(const char* restrict string, bool isNullTerminated, size_t length) {
    assert(string);
    const uint64_t FNV_OFFSET_BASIS = 0XCBF29CE484222325;
    const uint64_t FNV_PRIME        = 0X100000001B3;
    uint64_t       hash             = FNV_OFFSET_BASIS;

    if (isNullTerminated) {
        goto ifNulled;

    } else {
        for (size_t i = 0; i < length; ++i) {
            hash ^= string[i];
            hash *= FNV_PRIME;
        }
    }

ifNulled:
    while (string) {
        hash ^= *string++;
        hash *= FNV_PRIME;
    }

    return hash;
}

// true-words.txt has the following words repeated.
// Expect hash collisions.
/*
 * abacus
 * bidirectionally
 * goatee
 * leviathan
 * predrawn
 *
 */

int main(void) {
    HANDLE hFile = CreateFileA("./true-words.txt", GENERIC_READ | GENERIC_EXECUTE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    HANDLE hMemMappedFile = CreateFileMappingA(hFile, NULL, )
}
