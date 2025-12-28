#include "SequentialSolver.h"
#include "ParallelBruteForcer.h"
#include "BruteForceFactory.h"

namespace epuzzle::details
{
    std::unique_ptr<Solver> createBruteForcer(const SolverConfig::BruteForceConfig& config, PuzzleModel&& puzzleModel)
    {
        BruteForceContext context{ std::move(puzzleModel), config.prefilter };

        if (config.execution == SolverConfig::BruteForceConfig::ExecPolicy::Parallel)
        {
            return std::make_unique<ParallelBruteForcer>(std::move(context));
        }
        else
        {
            return std::make_unique<SequentialSolver>(std::move(context));
        }
    }
}
