#include "epuzzle/Exceptions.h"
#include "epuzzle/SolverConfig.h"

namespace epuzzle
{

#define ENSURE_CFG(expression, message) \
        ENSURE_SPEC(expression, SolverConfigError, message)

    namespace
    {
        using Method = SolverConfig::SolvingMethod;
        using ExecPolicy = SolverConfig::BruteForceConfig::ExecPolicy;
    }

    void validate(const SolverConfig& cfg)
    {
        if (cfg.solvingMethod == Method::BruteForce)
        {
            ENSURE_CFG(cfg.bruteForce.has_value(), "Must present for BruteForce solving method!");
            const auto execPolicy = cfg.bruteForce->execution;
            ENSURE_CFG(execPolicy == ExecPolicy::Parallel || execPolicy == ExecPolicy::Sequential,
                "Unexpected execution policy: (int)" << static_cast<int>(execPolicy));
        }
        else
        {
            ENSURE_CFG(cfg.solvingMethod == Method::Deductive, "Unexpected solving method: (int)" << static_cast<int>(cfg.solvingMethod));
        }
    }

    namespace
    {
        std::ostream& operator<<(std::ostream& os, Method method)
        {
            switch (method)
            {
            case Method::Deductive:     return os << "Deductive";
            case Method::BruteForce:    return os << "BruteForce";
            default: ENSURE(false, "Unsupported solving method type: (int) " << static_cast<int>(method));
            };
        }

        std::ostream& operator<<(std::ostream& os, ExecPolicy exec)
        {
            switch (exec)
            {
            case ExecPolicy::Parallel:  return os << "Parallel";
            case ExecPolicy::Sequential: return os << "Sequential";
            default: ENSURE(false, "Unsupported execution policy type: (int) " << static_cast<int>(exec));
            };
        }
    }

    std::ostream& operator<<(std::ostream& os, const SolverConfig& cfg)
    {
        os << "method = " << cfg.solvingMethod;
        if (cfg.bruteForce)
        {
            const auto& bf = cfg.bruteForce.value();
            os << ", prefilter = " << (bf.prefilter ? "true" : "false") << ", execution = " << bf.execution;
        }
        return os;
    }

}
