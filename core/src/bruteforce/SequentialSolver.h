#pragma once
#include "epuzzle/Solver.h"
#include "BruteForceContext.h"


namespace epuzzle::details
{
    // Solving in calling thread
    class SequentialSolver final : public Solver
    {
    public:
        explicit SequentialSolver(BruteForceContext&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        BruteForceContext m_ctx;
    };

}
