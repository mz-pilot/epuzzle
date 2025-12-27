#pragma once
#include "epuzzle/PuzzleSolution.h"
#include "PuzzleModel.h"

namespace epuzzle::details
{
    // Indexed (internal) version of puzzle solution
    struct SolutionModel
    {
        using AttributeAssignment = utils::IndexedVector<AttributeValueID, PersonID>;

        utils::IndexedVector<AttributeTypeID, AttributeAssignment> attributes;
    };

    PuzzleSolution toPuzzleSolution(const SolutionModel&, const PuzzleModel&);
}
