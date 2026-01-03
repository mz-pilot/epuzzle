#pragma once
#include "epuzzle/Solver.h"
#include "PuzzleModel.h"

namespace epuzzle::details::deductive
{
    // Placeholder for a future intelligent solver based on deductive logic and constraint propagation.
    // Currently under architectural design.
    class DeductiveSolver final : public Solver
    {
    public:
        explicit DeductiveSolver(PuzzleModel&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        const PuzzleModel m_puzzleModel;
    };

}
