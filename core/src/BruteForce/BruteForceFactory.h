#pragma once
#include "epuzzle/Solver.h"
#include "IndexedPuzzleData.h"

namespace epuzzle::details
{
    std::unique_ptr<Solver> createBruteForcer(const SolverConfig::BruteForce&, IndexedPuzzleData&&);
}
