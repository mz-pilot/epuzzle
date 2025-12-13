#pragma once
#include "epuzzle/ISolver.h"
#include "BruteForceContext.h"


namespace epuzzle::details
{
	// Checks all specified configurations against constraints.
	class BruteForcer final : public ISolver
	{
	public:
		explicit BruteForcer(BruteForceContext&&);

		std::vector<PuzzleSolution> solve(SolveOptions) override;

	private:
		BruteForceContext m_ctx;
	};

}