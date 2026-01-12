#pragma once
#include "epuzzle/Solver.h"
#include "SolverContext.h"

namespace epuzzle::details::bruteforce
{

    // Performs parallel exhaustive search in a thread pool using chunked range processing.
    class ParallelSolver final : public Solver
    {
    public:
        explicit ParallelSolver(SolverContext&&);

        std::vector<PuzzleSolution> solve(const SolveOptions&) override;

    private:
        std::vector<PuzzleSolution> runWorker(std::stop_token st, utils::AtomicProgressTracker&, class SpaceSplitter&) const;

        std::vector<PuzzleSolution> handleNoSolutionCandidates(const SolveOptions&) const;
        bool sendProgress(const SolveOptions&, std::uint64_t current) const;
        void handleProgressFinish(const SolveOptions&, std::uint64_t checked) const;

    private:
        const SolverContext m_ctx;
        const std::uint64_t m_totalSolutionCandidates;
    };

}
