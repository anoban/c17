// https://gist.githubusercontent.com/azrafe7/4089665/raw/b504384f9c82c0b622fb2c5d77ed00b4ec6cbe90/readChildOutput.c

// Slightly modified code from http://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
// Ma non capisco perché non mi termina dopo aver stampato l'output

// E comunque popen continua a non funzionarmi (CodeBlocks with MinGW - gcc 4.7.2)

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
// #include <strsafe.h>

#define BUFSIZE 4096

// Child output is written to this temp file and then read by this parent process (you may want to delete it when you're finished with it)
char*  tempFile           = "child.out";

// Child process cmd line
// TCHAR szCmdline[]=TEXT("c:\\windows\\system32\\getmac.exe");
TCHAR  szCmdline[]        = TEXT("cmd /c dir");

HANDLE g_hChildStd_IN_Rd  = NULL;
HANDLE g_hChildStd_IN_Wr  = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile       = NULL;

void   CreateChildProcess(void);
void   WriteToPipe(void);
void   ReadFromPipe(void);
void   ErrorExit(char*);

int    _tmain(int argc, TCHAR* argv[]) {
    SECURITY_ATTRIBUTES saAttr;

    printf("\n->Start of parent execution.\n");

    // Set the bInheritHandle flag so pipe handles are inherited.

    saAttr.nLength              = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle       = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT.

    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) ErrorExit("StdoutRd CreatePipe");

    // Ensure the read handle to the pipe for STDOUT is not inherited.

    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) ErrorExit("Stdout SetHandleInformation");

    // Create a pipe for the child process's STDIN.

    /*if (! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
      ErrorExit("Stdin CreatePipe");*/

    // Ensure the write handle to the pipe for STDIN is not inherited.

    /*if ( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
      ErrorExit("Stdin SetHandleInformation");*/

    // Create the child process.

    CreateChildProcess();

    // Get a handle to an input file for the parent.
    // This example assumes a plain text file and uses string output to verify data flow.

    g_hInputFile = CreateFile(tempFile, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_READONLY, NULL);

    if (g_hInputFile == INVALID_HANDLE_VALUE) ErrorExit("CreateFile");

    // Write to the pipe that is the standard input for a child process.
    // Data is written to the pipe's buffers, so it is not necessary to wait
    // until the child process is running before writing data.

    /*WriteToPipe();
   printf( "\n->Contents of %s written to child STDIN pipe.\n", argv[1]);*/

    // Read from pipe that is the standard output for child process.

    printf("\n->Contents of child process STDOUT: %s \n\n", tempFile);
    ReadFromPipe();

    printf("\n->End of parent execution.\n");

    // The remaining open handles are cleaned up when this process terminates.
    // To avoid resource leaks in a larger application, close handles explicitly.

    return 0;
}

void CreateChildProcess()
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO         siStartInfo;
    BOOL                bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure.

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure.
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb          = sizeof(STARTUPINFO);
    siStartInfo.hStdError   = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput  = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput   = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags    |= STARTF_USESTDHANDLES;

    // Create the child process.

    bSuccess                = CreateProcess(
        NULL,
        szCmdline,    // command line
        NULL,         // process security attributes
        NULL,         // primary thread security attributes
        TRUE,         // handles are inherited
        0,            // creation flags
        NULL,         // use parent's environment
        NULL,         // use parent's current directory
        &siStartInfo, // STARTUPINFO pointer
        &piProcInfo
    ); // receives PROCESS_INFORMATION

    // If an error occurs, exit the application.
    if (!bSuccess)
        ErrorExit(TEXT("CreateProcess"));
    else {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example.

        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
    }
}

void WriteToPipe(void)

// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data.
{
    DWORD dwRead, dwWritten;
    CHAR  chBuf[BUFSIZE];
    BOOL  bSuccess = FALSE;

    for (;;) {
        bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) break;

        bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
        if (!bSuccess) break;
    }

    // Close the pipe handle so the child process stops reading.

    if (!CloseHandle(g_hChildStd_IN_Wr)) ErrorExit(TEXT("StdInWr CloseHandle"));
}

void ReadFromPipe(void)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT.
// Stop when there is no more data.
{
    DWORD  dwRead, dwWritten;
    CHAR   chBuf[BUFSIZE];
    BOOL   bSuccess      = FALSE;
    HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    for (;;) {
        bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0) break;

        bSuccess = WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, NULL);
        if (!bSuccess) break;
    }
}

void ErrorExit(char* msg)

// Format a readable error message, display a message box,
// and exit from the application.
{
    printf("%s\n", msg);
    exit(1);
}
