#pragma once
#include "epuzzle/Solver.h"
#include "PuzzleModel.h"

namespace epuzzle::details
{
    std::unique_ptr<Solver> createBruteForcer(const SolverConfig::BruteForceConfig&, PuzzleModel&&);
}
