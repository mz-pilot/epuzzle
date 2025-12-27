#pragma once
#include "IndexedTypes.h"

namespace epuzzle::details
{
    // Stores original PuzzleDefinition, indexing it for best performance.
    class IndexedPuzzleData
    {
    public:
        explicit IndexedPuzzleData(PuzzleDefinition);

        size_t personCount() const;
        size_t attrTypeCount() const;
        const std::vector<IndexedConstraint>& constraintDefs() const;

        std::string_view personName(PersonID) const;
        std::string_view attrTypeName(AttributeTypeID) const;
        std::string_view attrValueName(AttributeTypeID, AttributeValueID) const;

    private:
        const PuzzleDefinition m_definition;
        std::vector<IndexedConstraint> m_indexedConstraints;
    };
}
