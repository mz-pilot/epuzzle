#pragma once
#include <new>

namespace utils
{
    constexpr size_t hwL1CacheLineSize()
    {
#ifdef __cpp_lib_hardware_interference_size
        return std::hardware_constructive_interference_size;
#else
        // 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
        return 64;
#endif
    }

}
