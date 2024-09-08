#define _AMD64_
#define AMD64

// clang-format off
#include <windef.h> // WIN32
#include <winnt.h>
#include <winternl.h>

#include <ntstatus.h>   // NT

#include <limits.h> // UCRT
// clang-format on

#pragma comment(lib, "ntdll.lib")

#if !defined(__clang__) && !defined(__llvm__) && (__STDC_VERSION__ != 202311L) // we need clang and -std=c23
    #define nullptr ((void*) 0)
#endif

NTSTATUS NTAPI NtTerminateProcess(_In_opt_ HANDLE hProcessHanlde, _In_ NTSTATUS lExitStatus); // undocumented by MS

int main(void) {
    // let's open and print the contents of a text file using only the native API
    // not using the C/C++ runtime

    HANDLE64                hFile               = nullptr;
    wchar_t                 pwszPath[CHAR_MAX]  = L"/??/D:/c17/iris.data"; // needs to be a fully qualified path
    UINT8                   bEaBuffer[CHAR_MAX] = { 0 };
    const OBJECT_ATTRIBUTES FileAttrs           = { .Length                   = sizeof(OBJECT_ATTRIBUTES),
                                                    .RootDirectory            = nullptr, // optional
                                                    .ObjectName               = pwszPath,
                                                    .Attributes               = OBJ_CASE_INSENSITIVE, // why not, but not needed though
                                                    .SecurityDescriptor       = nullptr,              // optional
                                                    .SecurityQualityOfService = nullptr };
    IO_STATUS_BLOCK         sbStatus            = { 0 };

    if (STATUS_SUCCESS != NtCreateFile(
                              &hFile,
                              FILE_GENERIC_READ,
                              &FileAttrs,
                              &sbStatus,
                              0,
                              FILE_ATTRIBUTE_NORMAL,
                              0,
                              FILE_OPEN,
                              FILE_NON_DIRECTORY_FILE,
                              bEaBuffer,
                              CHAR_MAX
                          )) { }
}
