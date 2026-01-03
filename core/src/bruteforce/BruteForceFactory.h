#pragma once
#include "epuzzle/Solver.h"
#include "PuzzleModel.h"

namespace epuzzle::details::bruteforce
{
    std::unique_ptr<Solver> createSolver(const SolverConfig::BruteForceConfig&, PuzzleModel&&);
}
