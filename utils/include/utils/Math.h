#pragma once
#include <cstdint>

namespace utils
{

    // Warn! Unsafe, can overflow
    constexpr std::uint64_t factorial(std::uint64_t n)
    {
        std::uint64_t res = 1;
        for (std::uint64_t i = 2; i <= n; ++i)
            res *= i;
        return res;
    }
    static_assert(factorial(5) == 120);


    // Warn! Unsafe, can overflow
    constexpr std::uint64_t power(std::uint64_t base, std::uint64_t exp)
    {
        std::uint64_t res = 1;
        for (std::uint64_t i = 0; i < exp; ++i)
            res *= base;
        return res;
    }
    static_assert(power(2, 5) == 32);


    constexpr std::uint64_t operator"" _u64(unsigned long long value)
    {
        return static_cast<std::uint64_t>(value);
    }

}
