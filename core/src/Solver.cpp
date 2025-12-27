#include "bruteforce/BruteForceFactory.h"
#include "deductive/Reasoner.h"

namespace epuzzle
{

    std::unique_ptr<Solver> Solver::create(SolverConfig config, PuzzleDefinition puzzleDefinition)
    {
        validate(config);

        normalize(puzzleDefinition);
        validate(puzzleDefinition);
        details::PuzzleModel puzzleModel{ std::move(puzzleDefinition) };

        if (config.solvingMethod == SolverConfig::SolvingMethod::BruteForce)
        {
            return details::createBruteForcer(config.bruteForce.value(), std::move(puzzleModel));
        }
        else
        {
            return std::make_unique<details::Reasoner>(std::move(puzzleModel));
        }
    }

}
