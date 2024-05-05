// clang .\ntapi.c -Wall -Wextra -pedantic -O3 -std=c23 -o .\ntapi.exe -DNDEBUG -D_NDEBUG

#define _AMD64_     // architecture
#define EXIT_STATUS 0xAB

#define _NTTERMINATEPROCESS_DIRECTLY

#include <processthreadsapi.h>
#include <stdio.h>
#include <stdlib.h>

typedef signed int NTSTATUS;
#pragma comment(lib, "ntdll.lib")

// we need to provide this declaration as MSVC doesn't ship headers for undocumented native APIs
NTSTATUS NTAPI NtTerminateProcess(_In_opt_ HANDLE ProcessHandle, _In_ NTSTATUS ExitStatus);

static void __cdecl SayGoodbye(void) { _putws(L"GoodBye!"); }

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

int main(void) {
    atexit(SayGoodbye); // let's register a exit handler to see what happens in all these approaches

#if defined(_REGULAR_RETURN)

    return EXIT_STATUS;
    // the value returned by main() will be collected and used by the msvcrt.dll's or ucrt.dll's (default these days) exit() function
    // which will in trun call Win32's ExitProcess()
    // which will then invoke NtTerminateProcess()
    // which will make the syscall to ultimately terminate the current process!
}

#elif defined(_EXIT_DIRECTLY)               // what will happen if we call exit() directly

    exit(EXIT_STATUS);
}

#elif defined(_EXITPROCESS_DIRECTLY)        // calling Win32's ExitProcess directly

    ExitProcess(EXIT_STATUS); // no GoodBye this time as we have bypassed the language runtime and invoked ExitProcess directly!
}

#elif defined(_NTTERMINATEPROCESS_DIRECTLY) // going another layer down

    const HANDLE64 hSelf = GetCurrentProcess();
    NtTerminateProcess(hSelf, EXIT_STATUS);
    // invoking NtTerminateProcess impedes teardowns at many layers - at language runtime level and at Win32 runtime level
}

#endif

// most native API routines have APIs in the Win32 subsystem with names removed of the Nt prefix
// C language runtime does a lot of work before calling an application's main() or wmain()function
// with C++, invoking exit or abort or ExitProcess or NtTerminateProcess may prevent proper cleanup expected of destructors

// Windows supports three CPU instructions to escalate privelege level to Ring 0
// syscall, sysenter and int 2E
// int 2E - system interrupt (has some overhead)
// sysenter is intriduced later as a faster alternative
// syscall is the new improved alternative on Intel & AMD CPUs.

// over the time, many native API functions has been removed and newly introduced or modified by Microsoft
// Microsoft documents some of the native APIs, soley for the use in kernel drivers
// Inside the windows kernel, the name of the routine invoked by the native API has the same name
// i.e NtTerminateProcess from NT API uses a syscall instruction to call a routine named NtTerminateProcess inside the kernel space

// however, the function in the kernel space has ring 0 priveleges (runs in trusted mode)
// it doesn't do any argument validation, because by design that is outsourced to the routines in Win32 and NT runtimes
// this helps with performance

// some cognate kernel functions have a Zw prefix replacing the Nt prefix of their NT API counterparts.
// e.g NtTerminateProcess in NT API may have a ZwTerminateProcess or NtTerminateProcess in the kernel space
// calling an NT API function with a Nt prefix may lead to a routine prefixed with Zw or Nt being dispatched in kernel space
// e.g NtDoSomething -> NtDoSomething or ZwDoSomething in kernel space
