#include "IndexedPuzzleData.h"

namespace epuzzle::details
{
    IndexedPuzzleData::IndexedPuzzleData(PuzzleData data)
        : m_data(std::move(data))
    {
        // helper lambdas

        auto attrTypeID = [&attrs = m_data.attributes](std::string_view typeName)
            {
                if (typeName == PuzzleData::personTypeName)
                    return AttributeTypeID_person;

                const auto cit = std::ranges::find(attrs, typeName, &PuzzleData::AttributeDescription::type);
                ENSURE(cit != attrs.cend(), "unexpected attribute type: " << typeName);
                return AttributeTypeID::fromDistance(cit, attrs.cbegin());
            };

        auto attrValueID = [this](AttributeTypeID typeId, std::string_view valueName)
            {
                if (typeId == AttributeTypeID_person)
                {
                    const auto cit = std::ranges::find(m_data.persons, valueName);
                    ENSURE(cit != m_data.persons.cend(), "unexpected person name: " << valueName);
                    return AttributeValueID::fromDistance(cit, m_data.persons.cbegin());
                }

                const auto& values = m_data.attributes[typeId.value()].values;
                const auto cit = std::ranges::find(values, valueName);
                ENSURE(cit != values.cend(), "unexpected value name: " << valueName);
                return AttributeValueID::fromDistance(cit, values.cbegin());
            };

        auto indexedAttr = [this, attrTypeID, attrValueID](const PuzzleData::Attribute& attr) -> Attribute
            {
                const auto typeId = attrTypeID(attr.type);
                return Attribute{ typeId, attrValueID(typeId, attr.value) };
            };

        // Prepare indexed constraints definitions (m_indexedConstraints)

        m_indexedConstraints.reserve(m_data.constraints.size());
        for (const auto& constr : m_data.constraints)
        {
            std::visit(utils::overloaded
                {
                    [this, indexedAttr](const PuzzleData::Fact& fact)
                    {
                        auto indexedFactFirst = indexedAttr(fact.first);
                        if (AttributeTypeID_person == indexedFactFirst.typeId)
                        {
                            m_indexedConstraints.emplace_back(std::in_place_type<PersonProperty>,
                                PersonID{indexedFactFirst.valueId.value()}, indexedAttr(fact.second), fact.secondNegate);
                        }
                        else
                        {
                            m_indexedConstraints.emplace_back(std::in_place_type<SameOwner>,
                                std::move(indexedFactFirst), indexedAttr(fact.second), fact.secondNegate);
                        }
                    },
                    [this, indexedAttr, attrTypeID](const PuzzleData::Comparison& comp)
                    {
                        m_indexedConstraints.emplace_back(std::in_place_type<PositionComparison>,
                            indexedAttr(comp.first), indexedAttr(comp.second), attrTypeID(comp.compareBy), comp.relation);
                    },
                }, constr);
        }
    }

    size_t IndexedPuzzleData::personCount() const
    {
        return m_data.persons.size();
    }

    size_t IndexedPuzzleData::attrTypeCount() const
    {
        return m_data.attributes.size();
    }

    const std::vector<IndexedConstraint>& IndexedPuzzleData::constraintDefs() const
    {
        return m_indexedConstraints;
    }

    std::string_view IndexedPuzzleData::personName(PersonID id) const
    {
        return m_data.persons[id.value()];
    }

    std::string_view IndexedPuzzleData::attrTypeName(AttributeTypeID typeId) const
    {
        return m_data.attributes[typeId.value()].type;
    }

    std::string_view IndexedPuzzleData::attrValueName(AttributeTypeID typeId, AttributeValueID valueId) const
    {
        return m_data.attributes[typeId.value()].values[valueId.value()];
    }
}
