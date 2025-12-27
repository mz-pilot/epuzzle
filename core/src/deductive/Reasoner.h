#pragma once
#include "epuzzle/Solver.h"
#include "PuzzleModel.h"

namespace epuzzle::details
{
    // Deductive reasoner (human-like thinking)
    class Reasoner final : public Solver
    {
    public:
        explicit Reasoner(PuzzleModel&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        const PuzzleModel m_puzzleModel;
    };

}
