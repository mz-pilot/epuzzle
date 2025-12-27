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
            Reasoning
        };

        struct BruteForceConfig
        {
            enum class ExecPolicy
            {
                Sequenced,
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
