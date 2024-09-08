#pragma once

// project header.
// to be included in C source files.

#include <master.h> // for standard headers.

// prototypes from imio.c
void* open_image(_In_ const wchar_t* file_name, _Out_ uint64_t* nread_bytes);
bool  write_image(_In_ const uint8_t* restrict buffer, _In_ const uint64_t buffsize, _In_ const wchar_t* restrict file_name);

// prototypes from error.c
void imerror_show(_In_ const wchar_t* restrict failed_proc);
void display_message(_In_ const wchar_t* restrict msg);