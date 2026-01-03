#pragma once
#include "epuzzle/Solver.h"
#include "SolverContext.h"


namespace epuzzle::details::bruteforce
{
    // Performs sequential exhaustive search in the calling thread.
    class SequentialSolver final : public Solver
    {
    public:
        explicit SequentialSolver(SolverContext&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        SolverContext m_ctx;
    };

}
