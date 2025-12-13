#pragma once
#include "IndexedPuzzleData.h"
#include "BruteForceChecker.h"
#include "ICandidatesSpace.h"

namespace epuzzle::details
{

	class BruteForceContext
	{
	public:
		BruteForceContext(IndexedPuzzleData&& indexedData, bool usePrefiltering)
			: m_data(std::move(indexedData))
			, m_checker(m_data.attrTypeCount(), m_data.constraintDefs(), usePrefiltering)
		{
			using namespace std::placeholders;
			// see BruteForceChecker class description
			m_space = ICandidatesSpace::create(m_data.personCount(), m_data.attrTypeCount(), 
				std::bind(&BruteForceChecker::prefilterCheck, &m_checker, _1, _2, _3));
		}

		const IndexedPuzzleData& indexedData() const { return m_data; }
		const BruteForceChecker& checker() const { return m_checker; }
		const ICandidatesSpace& space() const { return *m_space; }

	private:
		IndexedPuzzleData m_data;
		BruteForceChecker m_checker;
		std::unique_ptr<ICandidatesSpace> m_space;
	};

}