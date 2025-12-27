#pragma once
#include "epuzzle/Solver.h"
#include "BruteForceContext.h"

namespace epuzzle::details
{
    // Parallel (multithreaded) version of BruteForcer
    class ParallelBruteForcer final : public Solver
    {
    public:
        explicit ParallelBruteForcer(BruteForceContext&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        std::vector<PuzzleSolution> handleNoCandidates(const SolveOptions&) const;
        void handleProgressFinish(const SolveOptions&, std::uint64_t progressResult) const;

    private:
        const BruteForceContext m_ctx;
    };

}
