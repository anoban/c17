// clang .\ntapi.c -Wall -Wextra -pedantic -O3 -std=c23 -o .\ntapi.exe -DNDEBUG -D_NDEBUG

#define _AMD64_     // architecture
#define EXIT_STATUS 0xFF

// clang-format off
#include <stdio.h>
#include <stdlib.h>
#include <processthreadsapi.h>
// clang-format on

// trying out examples from Roger Orr's ACCU conf 2019 Talk
// How do Windows user space applications communicate with the OS kernel?

// Windows has two modes
// 1) User mode
// 2) Kernel mode

// User mode applications are subject to certain restrictions - cannot access hardware directly, can only access memory it owns!
// this includes prohibition from using specialized CPU instructions, if a user space application tries to use them, a hardware exeption will be raised!
// Kernel mode applications, on the contrary, have direct access to hardware and can access the whole of the physical memory

// Intel CPUs have 4 priority levels :: 0, 1, 2 and 3
// User space applications request services from the OS kernel using system calls
// these system calls use special assembly instructions like syscall, sysenter or int (stands for interrupt)
// immediately after a system call invocation, the execution mode gets escalated to kernel mode
// the syscall request and its arguments are validated
// the kernel prepares and provides the requested service and returns a status code
// indicative of the status of the requested service provision

// all the Windows native API routines have a name prefixed with "Nt"
// Microsoft openly discourages programmers from using the native API directly, as it's subject to frequent undocumented changes
// and encourages to use the Win32 API, that's guranteed to remain stable! All the refactorings, bug fixes, and what not happens in the native API
// and MS exposes a safe and stable encapsulation layer for programmers, the Win32 API

// most function calls a user space programme makes can travel down to various depths
// call to a user defined routine will contain the execution control within the programme itself.

// if it makes a call to a function from the language runtime, e.g. msvcrt.dll or ucrt.dll
// the execution control will temporarily move to the library code and return back.

// if it calls a function (be it user defined or from language runtime) that needs system services
// the call needs to travel into the kernel space to collect the required service
// if so happens, the function will likely call a Win32 routine
// which will load kernel32.dll to broker the request to the kernel space
// the called routine in the kernel32.dll will then call the corresponding native api, using a call to ntdll.dll
// the chosen routine in the ntdll will finally make the syscall!
// and the control will be returned back to the user space application, through all the layers involved in making the syscall
// in the reverse order

// however, there are ways to sidestep these cascades of control change
// a user space application can directly call the necessary native APIs, overstepping the modiation by the language runtime and Win32 API

static void SayGoodbye(void) { _putws(L"GoodBye!"); }

#ifdef _REGULAR_RETURN
int main(void) {
    // let's register a exit handler to see what happens in all these approaches
    atexit(SayGoodbye);

    return EXIT_STATUS;
    // the value returned by main() will be collected and used by the msvcrt.dll's or ucrt.dll's (default these days) exit() function
    // which will in trun call Win32's ExitProcess()
    // which will then invoke NtTerminateProcess()
    // which will make the syscall to ultimately terminate the current process!
}
#endif

#ifdef _EXIT_DIRECTLY
// what will happen if we call exit() directly
int main(void) {
    // let's register a exit handler to see what happens in all these approaches
    atexit(SayGoodbye);
    exit(EXIT_STATUS);
}
#endif

#ifdef _EXITPROCESS_DIRECTLY
// calling Win32's ExitProcess directly
int main(void) {
    atexit(SayGoodbye);
    ExitProcess(EXIT_STATUS); // no GoodBye this time as we have bypassed the language runtime and invoked ExitProcess directly!
}
#endif

// #ifdef _NTTERMINATEPROCESS_DIRECTLY
// going another layer down
typedef signed int NTSTATUS;
#pragma comment(lib, "ntdll.lib")
// we need to provide this declaration as MSVC doesn't ship headers for undocumented native APIs
NTSTATUS NTAPI NtTerminateProcess(_In_opt_ HANDLE ProcessHandle, _In_ NTSTATUS ExitStatus);

int main(void) {
    atexit(SayGoodbye); // no goddbyes here either
    const HANDLE64 hCurrentProces = GetCurrentProcess();
    NtTerminateProcess(hCurrentProces, EXIT_STATUS);
    // invoking NtTerminateProcess impedes teardowns at many layers - language runtime and Win32 runtime
}
// #endif
