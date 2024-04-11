// refactored version of https://raw.githubusercontent.com/NVIDIA/cuda-samples/master/Samples/1_Utilities/deviceQuery/deviceQuery.cpp
// tailored for CUDA runtimes 12.3 and later on Windows

// clang .\deviceQuery.c -Wall -Wextra -pedantic -O3 -std=c23 -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.4\include" -L "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.4\lib\x64\" -o .\deviceQuery.exe

/* Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// This sample queries the properties of the CUDA devices present in the system via CUDA Runtime API.

#include <assert.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "cudart.lib") // CUDA runtime APIs
#pragma comment(lib, "cuda.lib")   // CUDA driver APIs

#define BYTES_PER_MB            (1024.0L * 1024.0L)
#define MAX_ANTICIPATED_DEVICES 4LLU
// define the maximum number of devices you expec a system to have, defaulting to 64 seems a little far fetched

static_assert(sizeof(struct cudaDeviceProp) == 1032); // because cudaDeviceProp is a really huge struct
// imagine the size 64 such structs would take up on the stack

// an error handler for CUDA runtime API calls
// NOTE THE CHECK TAKES PLACE ON A PRVALUE!
// THE RETURN VALUE OF API CALLS IS NOT BEING CAPTURED IN A VARIABLE
static void check(_In_ const cudaError_t status, _In_ const wchar_t* const function, _In_ const wchar_t* const file, _In_ const int line) {
    // handle if the status is not cudaSuccess
    if (status != cudaSuccess) {
        fwprintf_s(
            stderr,
            L"CUDA error in %s @ line %d  code = %d (%S) in call to \"%s\" \n",
            file,
            line,
            status,                   // cudaError_t is a typedef to cudaError, which is a plain C style enum
            cudaGetErrorName(status), // looks like NVIDIA isn't as keen as MS in providing wchar_t variants for all their APIs
            function
        );
        exit(EXIT_FAILURE); // avoiding heap allocations as this premature exit could potentially lead to memory leaks
    }
}

// this has to be a macro, with a constexpr alternative L#expression, __FILEW__, __LINE__ will expand in-line, will be of no
// practical use to us
#define checkCudaErrors(expression) check((expression), L#expression, __FILEW__, __LINE__)

// function copied and refactored from https://raw.githubusercontent.com/NVIDIA/cuda-samples/master/Common/helper_cuda.h
static int CoreCountFromSMLookup(_In_ const int major, _In_ const int minor) {
    // defines for GPU Architecture types (using the SM version to determine the # of cores per SM
    typedef struct sSMtoCores {
            int SM; // 0xMm (hexidecimal notation), M = SM Major version and m = SM minor version
            int Cores;
    } sSMtoCores;

    static const sSMtoCores nGpuArchCoresPerSM[] = {
        { 0x30, 192 },
        { 0x32, 192 },
        { 0x35, 192 },
        { 0x37, 192 },
        { 0x50, 128 },
        { 0x52, 128 },
        { 0x53, 128 },
        { 0x60,  64 },
        { 0x61, 128 },
        { 0x62, 128 },
        { 0x70,  64 },
        { 0x72,  64 },
        { 0x75,  64 },
        { 0x80,  64 },
        { 0x86, 128 },
        { 0x87, 128 },
        { 0x89, 128 },
        { 0x90, 128 },
        {   -1,  -1 }  // indicator - SM version not in the records
    };

    int index = 0;

    while (nGpuArchCoresPerSM[index].SM != -1) { // until we reach to the last sSMtoCores struct
        if (nGpuArchCoresPerSM[index].SM == ((major << 4) + minor)) return nGpuArchCoresPerSM[index].Cores;
        index++;
    }

    // if we don't find the values, we default use the previous one to run properly
    wprintf_s(L"MapSMtoCores for SM %d.%d is undefined.  Default to use %d Cores/SM\n", major, minor, nGpuArchCoresPerSM[index - 1].Cores);
    return nGpuArchCoresPerSM[index - 1].Cores;
}

int main() {
    _putws(L" CUDA Device Query (Runtime API) version (CUDART static linking)\n\n");

    int deviceCount = 0;

    checkCudaErrors(cudaGetDeviceCount(&deviceCount));

    // This function call returns 0 if there are no CUDA capable devices.
    if (!deviceCount)
        _putws(L"There are no available device(s) that support CUDA\n");
    else
        wprintf_s(L"Detected %d CUDA Capable device(s)\n", deviceCount);

    int                   driverVersion = 0, runtimeVersion = 0;
    struct cudaDeviceProp deviceProp = { 0 };

    for (int i = 0; i < deviceCount; ++i) {
        cudaSetDevice(i);                           // select device i for consideration
        cudaGetDeviceProperties_v2(&deviceProp, i); // collect device i's properties

        wprintf_s(L"\nDevice %d: \"%S\"\n", i, deviceProp.name);

        cudaDriverGetVersion(&driverVersion);
        cudaRuntimeGetVersion(&runtimeVersion);
        wprintf_s(
            L"  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n",
            driverVersion / 1000,
            (driverVersion % 100) / 10,
            runtimeVersion / 1000,
            (runtimeVersion % 100) / 10
        );
        wprintf_s(L"  CUDA Capability Major/Minor version number:    %d.%d\n", deviceProp.major, deviceProp.minor);

        wprintf_s(
            L"  Total amount of global memory:                 %.0Lf MBytes (%zu bytes)\n",
            deviceProp.totalGlobalMem / BYTES_PER_MB,
            deviceProp.totalGlobalMem
        );

        wprintf_s(
            L"  (%03d) Multiprocessors, (%03d) CUDA Cores/MP:    %d CUDA Cores\n",
            deviceProp.multiProcessorCount,
            CoreCountFromSMLookup(deviceProp.major, deviceProp.minor),
            CoreCountFromSMLookup(deviceProp.major, deviceProp.minor) * deviceProp.multiProcessorCount
        );
        wprintf_s(
            L"  GPU Max Clock rate:                            %.0Lf MHz (%0.2Lf GHz)\n",
            deviceProp.clockRate * 1e-3L,
            deviceProp.clockRate * 1e-6L
        );

        // This is supported in CUDA 5.0 (runtime API device properties)
        wprintf_s(L"  Memory Clock rate:                             %.0Lf Mhz\n", deviceProp.memoryClockRate * 1e-3L);
        wprintf_s(L"  Memory Bus Width:                              %d-bit\n", deviceProp.memoryBusWidth);

        // if the device does have a L2 cache,
        if (deviceProp.l2CacheSize) wprintf_s(L"  L2 Cache Size:                                 %d bytes\n", deviceProp.l2CacheSize);

        wprintf_s(
            L"  Maximum Texture Dimension Size (x,y,z)         1D=(%d), 2D=(%d, %d), 3D=(%d, %d, %d)\n",
            deviceProp.maxTexture1D,
            deviceProp.maxTexture2D[0],
            deviceProp.maxTexture2D[1],
            deviceProp.maxTexture3D[0],
            deviceProp.maxTexture3D[1],
            deviceProp.maxTexture3D[2]
        );
        wprintf_s(
            L"  Maximum Layered 1D Texture Size, (num) layers  1D=(%d), %d layers\n"
            L"  Maximum Layered 2D Texture Size, (num) layers  2D=(%d, %d), %d layers\n",
            deviceProp.maxTexture1DLayered[0],
            deviceProp.maxTexture1DLayered[1],
            deviceProp.maxTexture2DLayered[0],
            deviceProp.maxTexture2DLayered[1],
            deviceProp.maxTexture2DLayered[2]
        );

        wprintf_s(
            L"  Total amount of constant memory:               %zu bytes\n"
            L"  Total amount of shared memory per block:       %zu bytes\n"
            L"  Total shared memory per multiprocessor:        %zu bytes\n"
            L"  Total number of registers available per block: %d\n"
            L"  Warp size:                                     %d\n"
            L"  Maximum number of threads per multiprocessor:  %d\n"
            L"  Maximum number of threads per block:           %d\n",
            deviceProp.totalConstMem,
            deviceProp.sharedMemPerBlock,
            deviceProp.sharedMemPerMultiprocessor,
            deviceProp.regsPerBlock,
            deviceProp.warpSize,
            deviceProp.maxThreadsPerMultiProcessor,
            deviceProp.maxThreadsPerBlock
        );

        wprintf_s(
            L"  Max dimension size of a thread block  (x,y,z): (%d, %d, %d)\n"
            L"  Max dimension size of a grid size     (x,y,z): (%d, %d, %d)\n",
            deviceProp.maxThreadsDim[0],
            deviceProp.maxThreadsDim[1],
            deviceProp.maxThreadsDim[2],
            deviceProp.maxGridSize[0],
            deviceProp.maxGridSize[1],
            deviceProp.maxGridSize[2]
        );

        wprintf_s(
            L"  Maximum memory pitch:                          %zu bytes\n"
            L"  Texture alignment:                             %zu bytes\n",
            deviceProp.memPitch,
            deviceProp.textureAlignment
        );

        wprintf_s(
            L"  Concurrent copy and kernel execution:          %s with %d copy engine(s)\n",
            (deviceProp.deviceOverlap ? L"Yes" : L"No"),
            deviceProp.asyncEngineCount
        );

        wprintf_s(
            L"  Run time limit on kernels:                     %s\n"
            L"  Integrated GPU sharing Host Memory:            %s\n"
            L"  Support host page-locked memory mapping:       %s\n"
            L"  Alignment requirement for Surfaces:            %s\n"
            L"  Device has ECC support:                        %s\n",
            deviceProp.kernelExecTimeoutEnabled ? L"Yes" : L"No",
            deviceProp.integrated ? L"Yes" : L"No",
            deviceProp.canMapHostMemory ? L"Yes" : L"No",
            deviceProp.surfaceAlignment ? L"Yes" : L"No",
            deviceProp.ECCEnabled ? L"Enabled" : L"Disabled"
        );

        wprintf_s(
            L"  CUDA Device Driver Mode (TCC or WDDM):         %s\n",
            deviceProp.tccDriver ? L"TCC (Tesla Compute Cluster Driver)" : L"WDDM (Windows Display Driver Model)"
        );

        wprintf_s(
            L"  Device supports Unified Addressing (UVA):      %s\n"
            L"  Device supports Managed Memory:                %s\n"
            L"  Device supports Compute Preemption:            %s\n"
            L"  Supports Cooperative Kernel Launch:            %s\n"
            L"  Supports MultiDevice Co-op Kernel Launch:      %s\n",
            deviceProp.unifiedAddressing ? L"Yes" : L"No",
            deviceProp.managedMemory ? L"Yes" : L"No",
            deviceProp.computePreemptionSupported ? L"Yes" : L"No",
            deviceProp.cooperativeLaunch ? L"Yes" : L"No",
            deviceProp.cooperativeMultiDeviceLaunch ? L"Yes" : L"No"
        );

        wprintf_s(
            L"  Device PCI Domain ID / Bus ID / location ID:   %d / %d / %d\n",
            deviceProp.pciDomainID,
            deviceProp.pciBusID,
            deviceProp.pciDeviceID
        );

        static const wchar_t* const sComputeMode[] = {
            L"Default (multiple host threads can use cudaSetDevice() with device simultaneously)",
            L"Exclusive (only one host thread in one process is able to use cudaSetDevice() with this device)",
            L"Prohibited (no host thread can use cudaSetDevice() with this device)",
            L"Exclusive Process (many threads in one process is able to use cudaSetDevice() with this device)",
            L"Unknown",
            NULL
        };
        _putws(L"  Compute Mode:\n");
        wprintf_s(L"     < %s >\n", sComputeMode[deviceProp.computeMode]);
    }

    // if there are 2 or more GPUs, query to determine whether RDMA is supported
    if (deviceCount >= 2) {
        // dynamic allocation will probably be better here, but PERFORMANCE!!!
        static struct cudaDeviceProp devProps[MAX_ANTICIPATED_DEVICES]  = { 0 };
        static int                   P2PGpuIds[MAX_ANTICIPATED_DEVICES] = { 0 }; // this is an array of GPU ids capable of P2P
        int                          p2pCount                           = 0;

        for (int i = 0; i < deviceCount; i++) {
            checkCudaErrors(cudaGetDeviceProperties_v2(&devProps[i], i));

            // only boards based on Fermi or later can support P2P
            // on Windows (64-bit), the Tesla Compute Cluster driver for windows must be enabled to support this
            if ((devProps[i].major >= 2) && devProps[i].tccDriver) P2PGpuIds[p2pCount++] = i;
        }

        // show all the combinations of support P2P GPUs
        int can_access_peer = 0;

        if (p2pCount >= 2) {
            for (int i = 0; i < p2pCount; i++) {
                for (int j = 0; j < p2pCount; j++) {
                    if (P2PGpuIds[i] == P2PGpuIds[j]) continue;
                    checkCudaErrors(cudaDeviceCanAccessPeer(&can_access_peer, P2PGpuIds[i], P2PGpuIds[j]));
                    wprintf_s(
                        L"> Peer access from %S (GPU%d) -> %S (GPU%d) : %s\n",
                        devProps[P2PGpuIds[i]].name,
                        P2PGpuIds[i],
                        devProps[P2PGpuIds[j]].name,
                        P2PGpuIds[j],
                        can_access_peer ? L"Yes" : L"No"
                    );
                }
            }
        }
    }
    _putws(L"");
    wprintf_s(
        L"deviceQuery, CUDA Driver = CUDART, CUDA Driver Version = %d.%d, CUDA Runtime Version = %d.%d, Number of Devices = %d\nResult = PASS\n\n",
        driverVersion / 1000,
        (driverVersion % 1000) / 10,
        runtimeVersion / 1000,
        (runtimeVersion % 100) / 10,
        deviceCount
    );

    return EXIT_SUCCESS;
}
