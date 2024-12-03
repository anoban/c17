#define _AMD64_
#include <heapapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <synchapi.h> // Sleep

#define MEMSIZE 1073741824LLU // that's 1 Gigabyte

// https://learn.microsoft.com/en-us/windows/win32/memory/getting-process-heaps

int main(void) {
    const size_t nProcHeaps = GetProcessHeaps(0, NULL);
    wprintf_s(L"This process has %zu heaps\n", nProcHeaps);
    const HANDLE64 hDefaultProcHeap = GetProcessHeap();
    void* const    memory           = HeapAlloc(hDefaultProcHeap, 0, MEMSIZE);

    // without memset, the kernel wouldn't have given this process the 1 Gig memory it asked for.
    memset(memory, 255U, MEMSIZE);

    Sleep(60000); // now that's one minute
    HeapFree(hDefaultProcHeap, 0, memory);
    return EXIT_SUCCESS;
}
