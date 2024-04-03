#include <wchar.h>

// cl .\dynamic.c /Wall /O2 /Ot /std:c17 /c gives us  dynamic.obj
// link dynamic.obj /NOENTRY /DLL /EXPORT:<> gives us dynamic.dll, dynamic.exp & dynamic.lib
// <TU name>.lib is the export library produced by the linker, any programme that wishes to consume this DLL must link this export library

// Why DLLs?

// advantages ::
// many exes can share code with dynamic link libraries (DLLs)
// this results in lean exes compared to static linking.
// reduces disk space use
// reduces runtime overall memory use, read only pages of DLLs could be shared between processes.
// componentization of code
// improved serviceability (bug fixes, refactoring, security patches..) can be realized in the DLL itself, the consuming exe doesn't need to be
// updated in any war or form in contrast to having to recompile and re-link the exe with the static library.
// improved maintainability

// drawbacks ::
// DLLs have a performance overhead compared to static linking
// load time linking vs runtime linking
// the DLL needs to be ransacked with a strcmp like function to traverse the binary to find the needed function pointer.
// practically impossible to optimize code across DLL boundaries e.g LTCG, LTO, inlining etc won't work

// when code has to interoperate across a DLL boundary, it's better to explicitly specify the calling convention
long double __declspec(dllexport) __cdecl factorial(unsigned n) { // function exported from this TU
    long double result = 1.00L;
    if (!n) return 1.000L;
    for (unsigned i = n; i > 0; --i) result *= i;
    return result;
}

// using a different calling convention, without an explicit dllexport attribute
// if an explicit __declspec(dllexport) attribute is absent in the function signature, pass /EXPORT:<name> to the linker so it
// includes the given routine in the import library
unsigned long long __stdcall gcd(const unsigned long long a, const unsigned long long b) {
    for (unsigned long long min = a > b ? b : a; min > 0; --min)
        if (!(a % min) && !(b % min)) return min;
    return 1;
}

// variables with static storage cannot be exported by DLLs
__declspec(dllexport) const wchar_t* const greeting = L"Hi there!"; // look this up in the .rdata section of the DLL

// a regular variable can also be exported without the __declspec(dllexport) annotation if /EXPORT:NAME is provided to the linker.
// e.g. link dynamic.obj /NOENTRY /DLL /EXPORT:gcd /EXPORT:name
const char* const                          name     = "Anoban";

// structure of a binary DLL

// 1) DOS stub - from a time when Windows and DOS interoperated, completely useless now. Exists there for portability reasons
// 2) PE signature - PE\0\0 ('P','E', folowed by two null terminators)
// 3) COFF file header
// 4) Optional header - not really optional, has info about the DLL
// 5) Section headers - gives info on how and where to find specific types of DLL data
// 6) Sections - sections corresponding to the section headers described in (5)

// the DOS stub
// contains an ASCII string saying "!This program cannot be run in DOS mode." and some other gibberish

// PE signature
// at offset 0x000000C8, we have the PE signature followed by two null bytes.
// this offset for PE signature is defined at the offset 0x0000003C, and it is C8!
// to locate a PE signature in a DLL, look at the value at the offset 3C!

// dumpbin.exe <dllname> /headers will spit out the header information
// and this is what we got!
/*
Dump of file .\dynamic.dll

PE signature found  <---- means that dumpbin has recognized this as an executable file (not necessarily an .exe)

File Type: DLL

FILE HEADER VALUES  <---- parsed info from the COFF file header
            8664 machine (x64)
               3 number of sections
        660D43E2 time date stamp Wed Apr  3 17:26:18 2024   <---- timestamp of the DLL build, customizeable though
               0 file pointer to symbol table
               0 number of symbols
              F0 size of optional header
            2022 characteristics
                   Executable
                   Application can handle large (>2GB) addresses    <---- 64 bit binary, naturally so
                   DLL

OPTIONAL HEADER VALUES  <---- parsed info from the "Optional" header.
             20B magic # (PE32+)    <---- PE32 for 32 bit binaries and PE32+ for 64 bit binaries.
           14.39 linker version
             200 size of code
             400 size of initialized data
               0 size of uninitialized data
               0 entry point        <---- null entry point because we linked with /NOENTRY
            1000 base of code       <---- preferred base address
       180000000 image base (0000000180000000 to 0000000180003FFF)
            1000 section alignment
             200 file alignment
            6.00 operating system version
            0.00 image version
            6.00 subsystem version
               0 Win32 version
            4000 size of image
             400 size of headers
               0 checksum
               2 subsystem (Windows GUI)
             160 DLL characteristics
                   High Entropy Virtual Addresses
                   Dynamic base
                   NX compatible
          100000 size of stack reserve
            1000 size of stack commit
          100000 size of heap reserve
            1000 size of heap commit
               0 loader flags
              10 number of directories  <---- additional metadata on what's inside the DLL
            2030 [      58] RVA [size] of Export Directory
               0 [       0] RVA [size] of Import Directory
               0 [       0] RVA [size] of Resource Directory
               0 [       0] RVA [size] of Exception Directory
               0 [       0] RVA [size] of Certificates Directory
               0 [       0] RVA [size] of Base Relocation Directory
            2008 [      1C] RVA [size] of Debug Directory
               0 [       0] RVA [size] of Architecture Directory
               0 [       0] RVA [size] of Global Pointer Directory
               0 [       0] RVA [size] of Thread Storage Directory
               0 [       0] RVA [size] of Load Configuration Directory
               0 [       0] RVA [size] of Bound Import Directory
               0 [       0] RVA [size] of Import Address Table Directory
               0 [       0] RVA [size] of Delay Import Directory
               0 [       0] RVA [size] of COM Descriptor Directory
               0 [       0] RVA [size] of Reserved Directory

WHAT'S RVA ??? - Relative Virtual Address
How do we address data and routines stored in a DLL?
When a DLL gets loaded into a process's virtual memory, it will get a base address.
Imagine the loading process as a memcpy of the DLL contents into the process's memory
The address where the first byte from the DLL gets copied to is the base address!
But this memory mapping isn't as straightforward as it looks because if the process has a highly fragmented memory space or
has multiple threads running, each using large segmented chunks of process memory, the loader may not be able to map the DLL code
into the preferred base address! In such cases, the loader will find an alternative space to map the DLL contents.
In modern Windows systems this type of loading is actually intentional for security reasons as predictability of DLL load addresses can lead to
easier exploits!
So this begs the question, how do we know where the DLL data and routines are given that the DLL gets loaded at an arbitrary address.
To circumnavigate this, DLL's actually use RVAs to document the locations of their data and code.
i.e the RVA (relative virtual address) is used in reference to a base address.
Imagine a DLL gets loaded into a process's virtual address 0xFFA145D563
RVAs describe the memory locations of DLL data and code as offsets from the base address, which is decided at load time.
RVA is basically an offset from the beginning of the DLL!

E.g. if our gcd routine is at RVA 0x34, and the base address where the DLL got mapped into the process memory is 0xFFA145D563,
we could use 0xFFA145D563 + 0x34 as the function pointer for gcd.

<---------------- BEGINNING OF SECTION HEADERS ---------------->

SECTION HEADER #1
   .text name              <---- text section, contains the executable code
      FA virtual size      <---- the size of code in this section in bytes
    1000 virtual address (0000000180001000 to 00000001800010F9)     <---- contents will be mapped to base address + 1000
     200 size of raw data
     400 file pointer to raw data (00000400 to 000005FF)
       0 file pointer to relocation table
       0 file pointer to line numbers
       0 number of relocations
       0 number of line numbers
60000020 flags
         Code
         Execute Read       <---- needs to be mapped into process memory page that has execute and read privileges!

SECTION HEADER #2
  .rdata name               <---- section for read only data
     120 virtual size       <---- 120 bytes of read only data
    2000 virtual address (0000000180002000 to 000000018000211F)
     200 size of raw data
     600 file pointer to raw data (00000600 to 000007FF)
       0 file pointer to relocation table
       0 file pointer to line numbers
       0 number of relocations
       0 number of line numbers
40000040 flags
         Initialized Data
         Read Only          <---- needs to be mapped to a page with only read previleges!

  Debug Directories

        Time Type        Size      RVA  Pointer
    -------- ------- -------- -------- --------
    660D43E2 coffgrp       78 000020A8      6A8

SECTION HEADER #3
   .data name
       4 virtual size
    3000 virtual address (0000000180003000 to 0000000180003003)
     200 size of raw data
     800 file pointer to raw data (00000800 to 000009FF)
       0 file pointer to relocation table
       0 file pointer to line numbers
       0 number of relocations
       0 number of line numbers
C0000040 flags
         Initialized Data
         Read Write

  Summary

        1000 .data
        1000 .rdata
        1000 .text
*/
