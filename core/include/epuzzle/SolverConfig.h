#pragma once
#include <cstdint>
#include <optional>
#include <ostream>

namespace epuzzle
{

    struct SolverConfig
    {
        enum class SolvingMethod : std::uint8_t
        {
            BruteForce,
            Deductive
        };

        struct BruteForceConfig
        {
            enum class ExecPolicy : std::uint8_t
            {
                Sequential,
                Parallel
            };

            bool prefilter = true;
            ExecPolicy execution = ExecPolicy::Parallel;
        };

        SolvingMethod solvingMethod = SolvingMethod::BruteForce;
        std::optional<BruteForceConfig> bruteForce;
    };

    void validate(const SolverConfig&); // throw SolverConfigError

    std::ostream& operator<<(std::ostream&, const SolverConfig&);
}
