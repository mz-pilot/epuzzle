#pragma once
#include "epuzzle/SolverConfig.h"
#include "ICandidatesSpace.h"

namespace epuzzle::details
{
	class SpaceParallelDistributor
	{
	public:
		explicit SpaceParallelDistributor(std::uint64_t totalCandidates)
			: m_totalCandidates(totalCandidates)
		{
		}

		std::unique_ptr<ICandidateIterator> take(const ICandidatesSpace& space)
		{
			std::uint64_t first = 0;
			std::uint64_t count = 0;
			const std::uint64_t chunkSize = 10'000'000; // could live in config-file
			{
				std::lock_guard<std::mutex>lock(m_distributedGuard);
				if (m_distributed >= m_totalCandidates)
					return {};

				first = m_distributed;
				count = std::min(chunkSize, m_totalCandidates - m_distributed);
				m_distributed += count;
			}
			return space.createIterator(first, count);
		}

	private:
		const std::uint64_t m_totalCandidates;

		std::uint64_t m_distributed = 0;
		std::mutex m_distributedGuard;
	};
}