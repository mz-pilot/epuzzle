#pragma once
#include "epuzzle/Solver.h"
#include "IndexedPuzzleData.h"

namespace epuzzle::details
{
    // Deductive reasoner (human-like thinking)
    class Reasoner final : public Solver
    {
    public:
        explicit Reasoner(IndexedPuzzleData&&);

        std::vector<PuzzleSolution> solve(SolveOptions) override;

    private:
        const IndexedPuzzleData m_data;
    };

}
