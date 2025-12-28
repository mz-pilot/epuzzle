#include "SequentialSolver.h"
#include "ParallelSolver.h"
#include "BruteForceFactory.h"

namespace epuzzle::details::bruteforce
{
    std::unique_ptr<Solver> createSolver(const SolverConfig::BruteForceConfig& config, PuzzleModel&& puzzleModel)
    {
        SolverContext context{ std::move(puzzleModel), config.prefilter };

        if (config.execution == SolverConfig::BruteForceConfig::ExecPolicy::Parallel)
        {
            return std::make_unique<ParallelSolver>(std::move(context));
        }
        else
        {
            return std::make_unique<SequentialSolver>(std::move(context));
        }
    }
}
