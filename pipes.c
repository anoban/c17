// clang .\pipes.c -Wall -Wextra -pedantic -O3 -std=c23 -o .\pipes.exe
// gcc pipes.c -Wall -Wextra -Wpedantic -O3 -municode -o pipes.exe -std=c17

// clang-format off
#define _AMD64_
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <sal.h>
#include <windef.h>
#include <WinBase.h>
#include <Windows.h>

#pragma warning(disable : 4710)

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
// clang-format on

#define BUFF_SIZE          200LLU
#define STDOUT_BUFFER_SIZE 1024 * 1024LLU // 1 MiB
#define WAIT_MILLISECS     100LLU         // 0.1 second

// https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output?redirectedfrom=MSDN
// Pipes are used in communication between two different processes.
// There will be two pipes between two communicating processes.
// These pipes can be named or anonymous.
// These two pipes can be visualized like this:

// Parent process ------->  Child process      (pipe 1)
// Child process  ------->  Parent process     (pipe 2)

// Pipes are unidirectional, information can be relayed in one direction only.
// This is why we need two pipes for a bidirectional communication.

// Read end (right) of the pipe 1 serves as the stdin for the child process
// and the write end (left) serves as the stdout for the parent process.

// The read end of pipe 2 serves as the stdin for the parent process
// and the write end serves as the stdout for thr child process.

// The terms Parent & Child are used here because the second process is created by the first process!.

// The read end of one pipe serves as standard input for the child process,
// and the write end of the other pipe is the standard output for the child process.
// These pipe handles are specified in the STARTUPINFO structure,
// which makes them the standard handles inherited by the child process.

// The parent process uses the opposite ends of these two pipes to write to the child process's input
// and read from the child process's output.
// As specified in the SECURITY_ATTRIBUTES structure, these handles are also inheritable.
// However, these handles must not be inherited. Therefore, before creating the child process,
// the parent process uses the SetHandleInformation() function to ensure that the write handle for the child process's standard input
// and the read handle for the child process's standard output cannot be inherited.

// Note that here we are only interested in a oneway communication.
// We need the launcher process to read the child process's stdout.

static HANDLE hThisProcessStdin   = NULL;
static HANDLE hChildProcessStdout = NULL;

// hThisProcessStdin and hChildProcessStdout form the two ends of one pipe.
// hThisProcessStdout and hChildProcessStdin form the two ends of the other pipe.

BOOL LaunchProcess(_In_ LPCWSTR pwszCommandLine) {
    // Create a child process that uses the previously created pipes as stdin & stderr
    PROCESS_INFORMATION piChildProcInfo = { 0 };
    const STARTUPINFOW  siChildProc     = { .cb         = sizeof(STARTUPINFO),
                                            .hStdError  = hChildProcessStdout,
                                            .hStdOutput = hChildProcessStdout,
                                            .dwFlags    = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES,
                                            .wShowWindow =
                                                SW_HIDE }; // prevents cmd window from flashing. requires STARTF_USESHOWWINDOW in dwFlags.

    // Lookup: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw
    // Passing the .exe's name in lpApplicationName causes error 2. "The system cannot find the file specified"
    // Pass the whole string to the lpCommandLine.
    WCHAR pwszCommand[BUFF_SIZE]        = { 0 };
    wcscpy_s(pwszCommand, BUFF_SIZE, pwszCommandLine);

    const BOOL bSuccess = CreateProcessW(
        NULL, // assumes the executable is in path.
              // lpCommandline must be a modifiable string (wchar array)
              // Passing a constant string will raise an access violation exception.
        pwszCommand,
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &siChildProc,
        &piChildProcInfo
    );

    // If invocation failed,
    if (!bSuccess) {
        fwprintf_s(stderr, L"Error %lu in CreateProcessW.\n", GetLastError());
        return FALSE;
    }

    const DWORD dwWaitStatus = WaitForSingleObject(piChildProcInfo.hProcess, WAIT_MILLISECS);
    switch (dwWaitStatus) {
        case WAIT_ABANDONED :
            fputws(L"Mutex object was not released by the child thread before the caller thread terminated.", stderr);
            break;
        case WAIT_TIMEOUT : fputws(L"The time-out interval elapsed, and the object's state is nonsignaled.", stderr); break;
        case WAIT_FAILED :
            fwprintf_s(stderr, L"Error %lu: Wait failed.\n", GetLastError());
            break;
            // WAIT_OBJECT_0 0x00000000L -> The state of the specified object is signaled.
        default : break;
    }

    // Close handles to the child process and its primary thread.
    CloseHandle(piChildProcInfo.hProcess);
    CloseHandle(piChildProcInfo.hThread);

    return TRUE;
}

BOOL ReadChildProcStdout(LPSTR lpszWriteBuffer, DWORD dwBuffSize) {
    // If size(stdout) > size(buffer), write will happen until there's no space in the buffer.

    DWORD      dwnBytesRead = 0;
    const BOOL bSuccess     = ReadFile(hThisProcessStdin, lpszWriteBuffer, dwBuffSize, &dwnBytesRead, NULL);

    // Close the child's ends of the pipe.
    CloseHandle(hChildProcessStdout);

    return bSuccess;
}

int wmain(void) {
    // A struct to specify the security attributes of the pipes.
    const SECURITY_ATTRIBUTES saPipeSecAttrs = { .nLength              = sizeof(saPipeSecAttrs),
                                                 .bInheritHandle       = TRUE /* makes pipe handles inheritable */,
                                                 .lpSecurityDescriptor = NULL };

    // Creating Child process ------> Parent process pipe.
    if (!CreatePipe(&hThisProcessStdin, &hChildProcessStdout, &saPipeSecAttrs, 0)) {
        fwprintf_s(stderr, L"Error %lu in creating Child -> Parent pipe.\n", GetLastError());
        return EXIT_FAILURE;
    }

    // Make the parent process's handles uninheritable.
    if (!SetHandleInformation(hThisProcessStdin, HANDLE_FLAG_INHERIT, FALSE)) {
        fwprintf_s(stderr, L"Error %lu in disabling handle inheritance.\n", GetLastError());
        return EXIT_FAILURE;
    }

    const BOOL bLaunchStatus    = LaunchProcess(L"clang-format --dump-config");
    CHAR* const restrict buffer = malloc(STDOUT_BUFFER_SIZE);
    if (!buffer) {
        // handle it
    }

    if (!bLaunchStatus) {
        fwprintf_s(stderr, L"Error %lu in launching child process!\n", GetLastError());
        free(buffer);
        return EXIT_FAILURE;
    }

    const BOOL bRead = ReadChildProcStdout(buffer, STDOUT_BUFFER_SIZE);
    if (!bRead) {
        fwprintf_s(stderr, L"Error %lu in reading from child process's stdout.\n", GetLastError());
        free(buffer);
        return EXIT_FAILURE;
    }

    puts(buffer);
    free(buffer);
    return EXIT_SUCCESS;
}
