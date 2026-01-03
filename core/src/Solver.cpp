#include "bruteforce/BruteForceFactory.h"
#include "deductive/DeductiveSolver.h"

namespace epuzzle
{

    std::unique_ptr<Solver> Solver::create(SolverConfig config, PuzzleDefinition puzzleDefinition)
    {
        validate(config);

        normalize(puzzleDefinition);
        validate(puzzleDefinition);

        using namespace details;
        PuzzleModel puzzleModel{ std::move(puzzleDefinition) };

        if (config.solvingMethod == SolverConfig::SolvingMethod::BruteForce)
        {
            return bruteforce::createSolver(config.bruteForce.value(), std::move(puzzleModel));
        }
        else
        {
            return std::make_unique<deductive::DeductiveSolver>(std::move(puzzleModel));
        }
    }

}
