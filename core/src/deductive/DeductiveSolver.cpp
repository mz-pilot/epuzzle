#include "DeductiveSolver.h"

namespace epuzzle::details::deductive
{

    DeductiveSolver::DeductiveSolver(PuzzleModel&& puzzleModel)
        : m_puzzleModel(std::move(puzzleModel))
    {
    }

    std::vector<PuzzleSolution> DeductiveSolver::solve(SolveOptions)
    {
        // TODO: Implement it! Something like Backtracking + Branch Pruning. Instead of bruteforce::ConstraintChecker it will use ConstraintPropagator
        // 
        // And don't forget to remove GTEST_SKIP() from SolverTests::SetUp() and SolverRealPuzzleTests::SetUp()!

        throw std::logic_error("Deductive method not implemented yet! Coming soon!");
    }

}
