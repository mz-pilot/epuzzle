#include "BruteForcer.h"

namespace epuzzle::details
{

    BruteForcer::BruteForcer(BruteForceContext&& ctx)
        : m_ctx(std::move(ctx))
    {
    }

    std::vector<PuzzleSolution> BruteForcer::solve(SolveOptions opts)
    {
        std::vector<PuzzleSolution> solutions;
        constexpr auto minProgressCount = 1'000'000u;
        const auto totalCandidates = m_ctx.space().candidatesCount();
        utils::ProgressTracker tracker(totalCandidates, opts.progressInterval, minProgressCount, opts.progressCallback);

        if (auto itCandidate = m_ctx.space().createIterator(0, totalCandidates))
        {
            do
            {
                // Hot cycle!
                if (m_ctx.checker().mainCheck(*itCandidate)) [[unlikely]]
                {
                    solutions.push_back(toPuzzleSolution(itCandidate->getIndexedSolution(), m_ctx.indexedData()));
                }
                if (!tracker.update()) [[unlikely]] // user canceled
                    return solutions;
            } while (itCandidate->next());
        }
        else
            tracker.finish();

        return solutions;
    }
}
