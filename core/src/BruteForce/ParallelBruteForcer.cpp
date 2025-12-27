#include "SpaceParallelDistributor.h"
#include "ParallelBruteForcer.h"

namespace epuzzle::details
{

    ParallelBruteForcer::ParallelBruteForcer(BruteForceContext&& ctx)
        : m_ctx(std::move(ctx))
    {
    }

    std::vector<PuzzleSolution> ParallelBruteForcer::solve(SolveOptions opts)
    {
        const auto totalCandidates = m_ctx.searchSpace().candidatesCount();
        if (totalCandidates == 0)
            return handleNoCandidates(opts);

        opts.progressCallback(totalCandidates, 0);

        SpaceParallelDistributor spaceDistributor(totalCandidates);

        const std::uint64_t countInterval = 1'000'000;
        utils::AtomicProgressTracker atomicTracker{ countInterval };

        using ThreadResult = std::vector<PuzzleSolution>;
        const auto threadsCount = std::max(1u, std::jthread::hardware_concurrency());
        utils::ParallelExecutor<ThreadResult> executor{ threadsCount, [this, &spaceDistributor, &atomicTracker](std::stop_token st)
            {
                ThreadResult threadResult;
                auto localTracker = atomicTracker.getLocalTracker();

                const auto& validator = m_ctx.validator();
                while (auto itCandidate = spaceDistributor.take(m_ctx.searchSpace()))
                {
                    if (st.stop_requested()) [[unlikely]]
                        return threadResult;
                    do
                    {
                        // Hot cycle!
                        if (validator.isSolutionCandidateValid(*itCandidate)) [[unlikely]]
                        {
                            threadResult.push_back(toPuzzleSolution(itCandidate->getSolutionModel(), m_ctx.puzzleModel()));
                        }
                        localTracker.update();
                    } while (itCandidate->next());
                }
                return threadResult;
            } };

        bool canceled = false;
        while (!executor.waitFor(opts.progressInterval))
        {
            if (!opts.progressCallback(totalCandidates, atomicTracker.load()))
            {
                executor.request_stop();
                canceled = true;
                break;
            }
        }
        const auto consolidated = utils::join(executor.get()); // after that all threads finished, results or exceptions collected
        if (!canceled)
            handleProgressFinish(opts, atomicTracker.load());
        return consolidated;
    }

    std::vector<PuzzleSolution> ParallelBruteForcer::handleNoCandidates(const SolveOptions& opts) const
    {
        opts.progressCallback(1, 0);
        opts.progressCallback(1, 1);
        return {};
    }

    void ParallelBruteForcer::handleProgressFinish(const SolveOptions& opts, std::uint64_t progressResult) const
    {
        const auto totalCandidates = m_ctx.searchSpace().candidatesCount();
        opts.progressCallback(totalCandidates, totalCandidates);

        ENSURE(totalCandidates == progressResult, "All workers are finished but progress is not completed!" <<
            " totalCandidates = " << totalCandidates << ", progressResult = " << progressResult);
    }

}
