#include "Reasoner.h"

namespace epuzzle::details
{

    Reasoner::Reasoner(PuzzleModel&& puzzleModel)
        : m_puzzleModel(std::move(puzzleModel))
    {
    }

    std::vector<PuzzleSolution> Reasoner::solve(SolveOptions)
    {
        // TODO: Implement it! And don't forget remove GTEST_SKIP() from SolverTests::SetUp() and SolverRealPuzzleTests::SetUp()!
        throw std::logic_error("Reasoning method not implemented yet! Coming soon!");
    }

}
