#pragma once
#include "epuzzle/Solver.h"
#include "PuzzleModel.h"

namespace epuzzle::details::deductive
{
    // Deductive solver (human-like thinking)
    class DeductiveSolver final : public Solver
    {
    public:
        explicit DeductiveSolver(PuzzleModel&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        const PuzzleModel m_puzzleModel;
    };

}
