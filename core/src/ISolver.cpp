#include "BruteForce/BruteForceFactory.h"
#include "Reasoning/Reasoner.h"

namespace epuzzle
{

    std::unique_ptr<ISolver> ISolver::create(SolverConfig cfg, PuzzleData data)
    {
        validate(cfg);

        normalize(data);
        validate(data);
        details::IndexedPuzzleData indexedData{ std::move(data) };

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
