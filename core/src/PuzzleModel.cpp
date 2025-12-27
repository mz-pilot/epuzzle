#include "PuzzleModel.h"

namespace epuzzle::details
{
    PuzzleModel::PuzzleModel(PuzzleDefinition definition)
        : m_definition(std::move(definition))
    {
        // helper lambdas

        auto attrTypeID = [&attrs = m_definition.attributes](std::string_view typeName)
            {
                if (typeName == PuzzleDefinition::personTypeName)
                    return AttributeTypeID_person;

                const auto cit = std::ranges::find(attrs, typeName, &PuzzleDefinition::AttributeDescription::type);
                ENSURE(cit != attrs.cend(), "unexpected attribute type: " << typeName);
                return AttributeTypeID::fromDistance(cit, attrs.cbegin());
            };

        auto attrValueID = [this](AttributeTypeID typeId, std::string_view valueName)
            {
                if (typeId == AttributeTypeID_person)
                {
                    const auto cit = std::ranges::find(m_definition.persons, valueName);
                    ENSURE(cit != m_definition.persons.cend(), "unexpected person name: " << valueName);
                    return AttributeValueID::fromDistance(cit, m_definition.persons.cbegin());
                }

                const auto& values = m_definition.attributes[typeId.value()].values;
                const auto cit = std::ranges::find(values, valueName);
                ENSURE(cit != values.cend(), "unexpected value name: " << valueName);
                return AttributeValueID::fromDistance(cit, values.cbegin());
            };

        auto indexedAttr = [this, attrTypeID, attrValueID](const PuzzleDefinition::Attribute& attr) -> Attribute
            {
                const auto typeId = attrTypeID(attr.type);
                return Attribute{ typeId, attrValueID(typeId, attr.value) };
            };

        // Prepare PuzzleModel::m_constraints (indexed) from PuzzleDefinition::constraints (DTO)

        m_constraints.reserve(m_definition.constraints.size());
        for (const auto& constr : m_definition.constraints)
        {
            std::visit(utils::overloaded
                {
                    [this, indexedAttr](const PuzzleDefinition::Fact& fact)
                    {
                        auto indexedFactFirst = indexedAttr(fact.first);
                        if (AttributeTypeID_person == indexedFactFirst.typeId)
                        {
                            m_constraints.emplace_back(std::in_place_type<PersonProperty>,
                                PersonID{indexedFactFirst.valueId.value()}, indexedAttr(fact.second), fact.secondNegate);
                        }
                        else
                        {
                            m_constraints.emplace_back(std::in_place_type<SameOwner>,
                                std::move(indexedFactFirst), indexedAttr(fact.second), fact.secondNegate);
                        }
                    },
                    [this, indexedAttr, attrTypeID](const PuzzleDefinition::Comparison& comp)
                    {
                        m_constraints.emplace_back(std::in_place_type<PositionComparison>,
                            indexedAttr(comp.first), indexedAttr(comp.second), attrTypeID(comp.compareBy), comp.relation);
                    },
                }, constr);
        }
    }

    size_t PuzzleModel::personCount() const
    {
        return m_definition.persons.size();
    }

    size_t PuzzleModel::attrTypeCount() const
    {
        return m_definition.attributes.size();
    }

    const std::vector<ConstraintModel>& PuzzleModel::constraints() const
    {
        return m_constraints;
    }

    std::string_view PuzzleModel::personName(PersonID id) const
    {
        return m_definition.persons[id.value()];
    }

    std::string_view PuzzleModel::attrTypeName(AttributeTypeID typeId) const
    {
        return m_definition.attributes[typeId.value()].type;
    }

    std::string_view PuzzleModel::attrValueName(AttributeTypeID typeId, AttributeValueID valueId) const
    {
        return m_definition.attributes[typeId.value()].values[valueId.value()];
    }
}
