#pragma once
#include "IndexedSolution.h"

namespace epuzzle::details
{
	// Navigates through the candidate space, tracking progress, and provides access to the current candidate
	class ICandidateIterator
	{
	public:
		virtual ~ICandidateIterator() = default;

		[[nodiscard]] virtual bool next() = 0;

		virtual PersonID ownerOf(AttributeTypeID, AttributeValueID) const = 0;
		virtual size_t personPosition(PersonID, AttributeTypeID) const = 0;
		
		virtual IndexedSolution getIndexedSolution() const = 0;
	};

}
