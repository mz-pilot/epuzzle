#pragma once
#include "epuzzle/ISolver.h"
#include "IndexedPuzzleData.h"

namespace epuzzle::details
{
    std::unique_ptr<ISolver> createBruteForcer(const SolverConfig::BruteForce&, IndexedPuzzleData&&);
}
