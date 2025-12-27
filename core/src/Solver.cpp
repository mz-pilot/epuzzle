#include "BruteForce/BruteForceFactory.h"
#include "Reasoning/Reasoner.h"

namespace epuzzle
{

    std::unique_ptr<Solver> Solver::create(SolverConfig cfg, PuzzleDefinition puzzle)
    {
        validate(cfg);

        normalize(puzzle);
        validate(puzzle);
        details::IndexedPuzzleData indexedData{ std::move(puzzle) };

        if (cfg.solvingMethod == SolverConfig::SolvingMethod::BruteForce)
        {
            return details::createBruteForcer(cfg.bruteForce.value(), std::move(indexedData));
        }
        else
        {
            return std::make_unique<details::Reasoner>(std::move(indexedData));
        }
    }

}
