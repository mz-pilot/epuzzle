#include "SequentialSolver.h"

namespace epuzzle::details::bruteforce
{

    SequentialSolver::SequentialSolver(SolverContext&& ctx)
        : m_ctx(std::move(ctx))
    {
    }

    std::vector<PuzzleSolution> SequentialSolver::solve(const SolveOptions& opts)
    {
        std::vector<PuzzleSolution> solutions;
        constexpr auto minCountInterval = 1'000'000u;
        utils::ProgressTracker tracker(m_ctx.searchSpace().totalSolutionCandidates(), opts.progressInterval, minCountInterval, opts.progressCallback);

        if (auto cursor = m_ctx.searchSpace().createCursor())
        {
            do
            {
                // Hot cycle!
                if (m_ctx.validator().isSolutionValid(*cursor)) [[unlikely]]
                {
                    solutions.push_back(cursor->getSolutionModel().toPuzzleSolution(m_ctx.puzzleModel()));
                }
                if (!tracker.update()) [[unlikely]] // user canceled
                    return solutions;

            } while (cursor->moveNext());
        }
        else
            tracker.finish();

        return solutions;
    }
}
