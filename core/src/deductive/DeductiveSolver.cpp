#include "DeductiveSolver.h"

namespace epuzzle::details
{

    DeductiveSolver::DeductiveSolver(PuzzleModel&& puzzleModel)
        : m_puzzleModel(std::move(puzzleModel))
    {
    }

    std::vector<PuzzleSolution> DeductiveSolver::solve(SolveOptions)
    {
        // TODO: Implement it! And don't forget remove GTEST_SKIP() from SolverTests::SetUp() and SolverRealPuzzleTests::SetUp()!
        throw std::logic_error("Deductive method not implemented yet! Coming soon!");
    }

}
