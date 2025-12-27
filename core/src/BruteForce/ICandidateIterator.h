#pragma once
#include "SolutionModel.h"

namespace epuzzle::details
{
    // Navigates through the SearchSpace and provides access to the current "solution candidate"
    class ICandidateIterator
    {
    public:
        virtual ~ICandidateIterator() = default;

        [[nodiscard]] virtual bool next() = 0;

        virtual PersonID ownerOf(AttributeTypeID, AttributeValueID) const = 0;
        virtual size_t personPosition(PersonID, AttributeTypeID) const = 0;

        virtual SolutionModel getSolutionModel() const = 0;
    };

}
