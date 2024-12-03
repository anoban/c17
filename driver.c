#include <ntddk.h>

// at one point the kernel will hve to unload the driver,
// at this time, the driver process should do a cleanup to avoid resource leaks
// any resources acquired for use through DriverEntry() must be released back to the OS
// RESOURCE LEAKS CREATED THIS WAY WILL NOT BE CLEANED UP BY THE KERNEL UNTIL THE NEXT REBOOT!

void __stdcall AtDriverUnload(_In_ PDRIVER_OBJECT DriverObject) { UNREFERENCED_PARAMETER(DriverObject); }

// All Windows drivers must define a DriverEntry function
// this is the entry point where the Windows kernel starts executing the driver code

NTSTATUS __stdcall DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
    // by default for drivers MSVC uses /WX, so if the compilation generates any warnings, it will ultimately fail!
    // we could disable /WX but it is a good practice to keep it, because device drivers are critical pieces that
    // interact closely with the kernel, it's best to write code that does not generate any warnings!

    // this suppresses warning C4100: '<argument>': unreferenced formal parameter
    // UNREFERENCED_PARAMETER(DriverObject);
    DriverObject->DriverUnload = AtDriverUnload; // the function to be invoked to do the cleanup when the kernel unloads the driver
    UNREFERENCED_PARAMETER(RegistryPath);

    return STATUS_SUCCESS;
}

// compiled drivers have a .sys extension NOT .exe
