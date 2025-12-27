#pragma once
#include "epuzzle/Solver.h"
#include "BruteForceContext.h"


namespace epuzzle::details
{
    // Checks all specified configurations against constraints.
    class BruteForcer final : public Solver
    {
    public:
        explicit BruteForcer(BruteForceContext&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        BruteForceContext m_ctx;
    };

}
