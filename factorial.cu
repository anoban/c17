#include <algorithm>
#include <cstdlib>
#include <execution>
#include <iostream>
#include <numeric>
#include <vector>

#ifdef RAND_MAX
    #undef RAND_MAX
    #define RAND_MAX 100LLU
#endif

__global__ void factkernel(_In_ long double* const d_factorials, _In_ const int32_t* const d_randoms, _In_ const size_t d_size) {
    long double result { 1.000000 };
    const auto  dev_thread = threadIdx.x + threadIdx.y + threadIdx.z;
    for (size_t i = d_randoms[dev_thread]; i > 0; --i) result *= i;
    d_factorials[dev_thread] = result;
    return;
}

constexpr size_t NRANDOMS { 100'000'000 };
constexpr size_t SIZERANDOMS { NRANDOMS * sizeof(int32_t) }; // 400,000,000 bytes (about 38 MiBs)

auto             main(void) -> int {
    srand(time(nullptr));
    std::vector<int32_t> randoms(NRANDOMS);
    std::for_each(randoms.begin(), randoms.end(), [](int32_t& e) {
        e = rand();
        return;
    });

    // store the sum for later comparison
    const auto sum = std::reduce(std::execution::par, randoms.cbegin(), randoms.cend(), 0LLU, std::plus {});

    // device operations
    int32_t*   device_randoms { nullptr };
    cudaMalloc(&device_randoms, SIZERANDOMS);
    cudaMemcpy(device_randoms, randoms.data(), SIZERANDOMS, cudaMemcpyHostToDevice);
    return EXIT_SUCCESS;
}
