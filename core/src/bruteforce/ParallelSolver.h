#pragma once
#include "epuzzle/Solver.h"
#include "SolverContext.h"

namespace epuzzle::details::bruteforce
{

    // Solving in thread pool
    class ParallelSolver final : public Solver
    {
    public:
        explicit ParallelSolver(SolverContext&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        std::vector<PuzzleSolution> handleNoCandidates(const SolveOptions&) const;
        void handleProgressFinish(const SolveOptions&, std::uint64_t progressResult) const;

    private:
        const SolverContext m_ctx;
    };

}
