#pragma once
#include "epuzzle/ISolver.h"
#include "IndexedPuzzleData.h"

namespace epuzzle::details
{
	// Deductive reasoner (human-like thinking)
	class Reasoner final : public ISolver
	{
	public:
		explicit Reasoner(IndexedPuzzleData&&);

		std::vector<PuzzleSolution> solve(SolveOptions) override;

	private:
		const IndexedPuzzleData m_data;
	};

}