#pragma once
#include "epuzzle/PuzzleSolution.h"
#include "IndexedPuzzleData.h"

namespace epuzzle::details
{
    // Indexed (internal) version of puzzle solution
    struct IndexedSolution
    {
        using AttributeAssignment = utils::IndexedVector<AttributeValueID, PersonID>;

        utils::IndexedVector<AttributeTypeID, AttributeAssignment> attributes;
    };

    PuzzleSolution toPuzzleSolution(const IndexedSolution&, const IndexedPuzzleData&);
}
