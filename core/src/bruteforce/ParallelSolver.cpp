#include "SpaceSplitter.h"
#include "ParallelSolver.h"

namespace epuzzle::details::bruteforce
{
    namespace
    {
        constexpr std::uint64_t progressCountInterval = 1'000'000;
        static const auto threadsCount = std::max(1u, std::jthread::hardware_concurrency());
    }

    ParallelSolver::ParallelSolver(SolverContext&& ctx)
        : m_ctx(std::move(ctx))
        , m_totalCombinations(m_ctx.searchSpace().totalCombinations())
    {
    }

    std::vector<PuzzleSolution> ParallelSolver::solve(SolveOptions opts)
    {
        if (m_totalCombinations == 0)
            return handleNoCombinations(opts);

        opts.progressCallback(m_totalCombinations, 0);

        utils::AtomicProgressTracker atomicTracker{ progressCountInterval };
        SpaceSplitter spaceSplitter{ m_totalCombinations };

        utils::ParallelExecutor<std::vector<PuzzleSolution>> executor{ threadsCount, [this, &atomicTracker, &spaceSplitter](std::stop_token st)
            {
                return runWorker(st, atomicTracker, spaceSplitter);
            } };

        bool canceled = false;
        while (!executor.waitFor(opts.progressInterval))
        {
            if (!opts.progressCallback(m_totalCombinations, atomicTracker.load()))
            {
                canceled = true;
                executor.request_stop();
                break;
            }
        }

        // After `executor.get()` all threads finished, results or exceptions collected
        const auto joinedResult = utils::join(executor.get());

        if (!canceled)
            handleProgressFinish(opts, atomicTracker.load());

        return joinedResult;
    }

    // parallel
    std::vector<PuzzleSolution> ParallelSolver::runWorker(std::stop_token st, utils::AtomicProgressTracker& atomicTracker, SpaceSplitter& spaceSplitter) const
    {
        std::vector<PuzzleSolution> threadResult;
        auto localTracker = atomicTracker.getLocalTracker();
        const auto& validator = m_ctx.validator();

        while (auto chunk = spaceSplitter.nextChunk())
        {
            auto cursor = m_ctx.searchSpace().createCursor(chunk->offset, chunk->count);
            ENSURE(cursor, "cursor must be created!");

            if (st.stop_requested()) [[unlikely]]
                return threadResult;
            do
            {
                // Hot cycle!
                if (validator.isSolutionValid(*cursor)) [[unlikely]]
                {
                    threadResult.push_back(cursor->getSolutionModel().toPuzzleSolution(m_ctx.puzzleModel()));
                }
                localTracker.update();
            } while (cursor->moveNext());
        }
        return threadResult;
    }

    std::vector<PuzzleSolution> ParallelSolver::handleNoCombinations(const SolveOptions& opts) const
    {
        opts.progressCallback(1, 0);
        opts.progressCallback(1, 1);
        return {};
    }

    void ParallelSolver::handleProgressFinish(const SolveOptions& opts, std::uint64_t checkedCombinations) const
    {
        opts.progressCallback(m_totalCombinations, m_totalCombinations);

        ENSURE(m_totalCombinations == checkedCombinations, "All workers are finished but progress is not completed!"
            << " total: " << m_totalCombinations << ", checked: " << checkedCombinations);
    }

}
