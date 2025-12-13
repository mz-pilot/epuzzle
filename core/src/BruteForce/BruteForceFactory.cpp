#include "BruteForcer.h"
#include "ParallelBruteForcer.h"
#include "BruteForceFactory.h"

namespace epuzzle::details
{
	std::unique_ptr<ISolver> createBruteForcer(const SolverConfig::BruteForce& bfConfig, IndexedPuzzleData&& indexedData)
	{
		BruteForceContext context{ std::move(indexedData), bfConfig.prefilter };

		if (bfConfig.execution == SolverConfig::BruteForce::ExecPolicy::Parallel)
		{
			return std::make_unique<ParallelBruteForcer>(std::move(context));
		}
		else
		{
			return std::make_unique<BruteForcer>(std::move(context));
		}
	}
}