#include <iostream>
#include <string>

#include <cuda.h>
#pragma comment(lib, "cuda.lib") // the functions used here come from the CUDA Driver API!!
// linker needs to link the driver API instead of the runtime API!

int main(void) {
    cuInit(NULL);

    int ndevices {};
    cuDeviceGetCount(&ndevices);
    if (ndevices > 1) exit(-1);

    char     device_name[256] {};

    CUdevice device;
    cuDeviceGet(&device, 0); // we don't have multiple GPUs here
    cuDeviceGetName(device_name, __crt_countof(device_name), device);

    std::cout << "There are " << ndevices << " NVIDIA CUDA capable GPU's on this machine\n";
    std::cout << "Name of the GPU is " << device_name << std::endl;

    return EXIT_SUCCESS;
}
