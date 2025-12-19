#pragma once
#include "IndexedTypes.h"

namespace epuzzle::details
{
    // Stores original PuzzleData, indexing it for best performance.
    class IndexedPuzzleData
    {
    public:
        explicit IndexedPuzzleData(PuzzleData);

        size_t personCount() const;
        size_t attrTypeCount() const;
        const std::vector<IndexedConstraint>& constraintDefs() const;

        std::string_view personName(PersonID) const;
        std::string_view attrTypeName(AttributeTypeID) const;
        std::string_view attrValueName(AttributeTypeID, AttributeValueID) const;

    private:
        const PuzzleData m_data;
        std::vector<IndexedConstraint> m_indexedConstraints;
    };
}
