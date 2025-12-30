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
        void setAttributeAssignment(AttributeTypeID, Assignment);

        PuzzleSolution toPuzzleSolution(const PuzzleModel&) const;

    private:
        utils::IndexedVector<AttributeTypeID, Assignment> m_attributes;
    };
}
