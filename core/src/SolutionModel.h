#pragma once
#include "epuzzle/PuzzleSolution.h"
#include "PuzzleModel.h"

namespace epuzzle::details
{
    // Indexed (internal) version of puzzle solution
    class SolutionModel
    {
    public:
        explicit SolutionModel(size_t attrTypeCount);
        void setAttributeAssignment(AttributeTypeID, AttributeAssignment);

        PuzzleSolution toPuzzleSolution(const PuzzleModel&) const;

    private:
        utils::IndexedVector<AttributeTypeID, AttributeAssignment> m_attributes;
    };
}
