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
        , m_totalSolutionCandidates(m_ctx.searchSpace().totalSolutionCandidates())
    {
    }

    std::vector<PuzzleSolution> ParallelSolver::solve(SolveOptions opts)
    {
        if (m_totalSolutionCandidates == 0)
            return handleNoSolutionCandidates(opts);

        sendProgress(opts, 0);

        utils::AtomicProgressTracker atomicTracker{ progressCountInterval };
        SpaceSplitter spaceSplitter{ m_totalSolutionCandidates };

        utils::ParallelExecutor<std::vector<PuzzleSolution>> executor{ threadsCount, [this, &atomicTracker, &spaceSplitter](std::stop_token st)
            {
                return runWorker(st, atomicTracker, spaceSplitter);
            } };

        bool userCanceled = false;
        while (!executor.waitFor(opts.progressInterval))
        {
            if (!sendProgress(opts, atomicTracker.load())) [[unlikely]]
            {
                userCanceled = true;
                executor.request_stop();
                break;
            }
        }

        const auto joinedResult = utils::join(executor.collectResults()); // here all worker-threads finished

        if (!userCanceled)
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

    std::vector<PuzzleSolution> ParallelSolver::handleNoSolutionCandidates(const SolveOptions& opts) const
    {
        opts.progressCallback(1, 0);
        opts.progressCallback(1, 1);
        return {};
    }

    bool ParallelSolver::sendProgress(const SolveOptions& opts, std::uint64_t current) const
    {
        return opts.progressCallback(m_totalSolutionCandidates, current);
    }

    void ParallelSolver::handleProgressFinish(const SolveOptions& opts, std::uint64_t checked) const
    {
        sendProgress(opts, checked);
        ENSURE(checked == m_totalSolutionCandidates, "All workers are finished but progress is not completed!"
            << " checked: " << checked << ", total: " << m_totalSolutionCandidates);
    }

}
