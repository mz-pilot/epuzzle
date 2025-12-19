#include "Reasoner.h"

namespace epuzzle::details
{

    Reasoner::Reasoner(IndexedPuzzleData&& data)
        : m_data(std::move(data))
    {
    }

    std::vector<PuzzleSolution> Reasoner::solve(SolveOptions)
    {
        // TODO: Implement it! And don't forget remove GTEST_SKIP() from ISolverTests::SetUp() and ISolverRealPuzzleTests::SetUp()!
        throw std::logic_error("Reasoning method not implemented yet! Coming soon!");
    }

}
