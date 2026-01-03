#pragma once
#include "SolutionModel.h"

namespace epuzzle::details::bruteforce
{
    // Navigates through the SearchSpace in some range and provides access to the current solution candidate. 
    class SearchSpaceCursor
    {
    public:
        virtual ~SearchSpaceCursor() = default;

        // Move to next solution candidate
        [[nodiscard]] virtual bool moveNext() = 0;

        virtual PersonID ownerOf(AttributeTypeID, AttributeValueID) const = 0;
        virtual size_t personPosition(PersonID, AttributeTypeID) const = 0;

        // Get current combination as solution
        virtual SolutionModel getSolutionModel() const = 0;
    };

}
