#pragma once
#include <optional>
#include <ostream>

namespace epuzzle
{

    struct SolverConfig
    {
        enum class SolvingMethod
        {
            BruteForce,
            Deductive
        };

        struct BruteForceConfig
        {
            enum class ExecPolicy
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
