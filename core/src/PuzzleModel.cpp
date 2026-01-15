#include "PuzzleModel.h"

namespace epuzzle::details
{

    PuzzleModel::PuzzleModel(PuzzleDefinition definition)
        : m_definition(std::move(definition))
    {
        // helper lambdas

        auto indexedPerson = [this](std::string_view valueName)
            {
                const auto cit = std::ranges::find(m_definition.persons, valueName);
                ENSURE(cit != m_definition.persons.cend(), "unexpected person name: " << valueName);
                return PersonID::fromDistance(cit, m_definition.persons.cbegin());
            };

        auto attrTypeID = [this](std::string_view typeName)
            {
                const auto cit = std::ranges::find(m_definition.attributes, typeName, &PuzzleDefinition::AttributeDescription::type);
                ENSURE(cit != m_definition.attributes.cend(), "unexpected attribute type: " << typeName);
                return AttributeTypeID::fromDistance(cit, m_definition.attributes.cbegin());
            };

        auto indexedAttr = [this, attrTypeID](const PuzzleDefinition::Attribute& attr) -> Attribute
            {
                const auto typeId = attrTypeID(attr.type);
                const auto valueName = attr.value;

                const auto& values = m_definition.attributes[typeId.value()].values;
                const auto cit = std::ranges::find(values, valueName);
                ENSURE(cit != values.cend(), "unexpected value name: " << valueName);

                return Attribute{ .typeId = typeId, .valueId = AttributeValueID::fromDistance(cit, values.cbegin()) };
            };

        auto indexedComparable = [indexedPerson, indexedAttr](const PuzzleDefinition::Attribute& attr) -> std::variant<PersonID, Attribute>
            {
                if (attr.type == PuzzleDefinition::personTypeName)
                    return indexedPerson(attr.value);

                return indexedAttr(attr);
            };

        // PuzzleDefinition::constraints (input DTO)  ->  PuzzleModel::m_constraints (optimized model)

        m_constraints.reserve(m_definition.constraints.size());
        for (const auto& constr : m_definition.constraints)
        {
            std::visit(utils::overloaded
                {
                    [this, indexedAttr, indexedPerson](const PuzzleDefinition::Fact& fact)
                    {
                        // Note: if we have a Fact with a `person` attribute, it can only be the first attribute
                        // (see `normalize(PuzzleDefinition&)` and `validate(const PuzzleDefinition&)` functions
                        if (fact.first.type == PuzzleDefinition::personTypeName)
                        {
                            m_constraints.emplace_back(std::in_place_type<PersonProperty>,
                                indexedPerson(fact.first.value), indexedAttr(fact.second), fact.secondNegate);
                        }
                        else
                        {
                            m_constraints.emplace_back(std::in_place_type<SameOwner>,
                                indexedAttr(fact.first), indexedAttr(fact.second), fact.secondNegate);
                        }
                    },
                    [this, attrTypeID, indexedComparable](const PuzzleDefinition::Comparison& comp)
                    {
                        m_constraints.emplace_back(std::in_place_type<PositionComparison>,
                            indexedComparable(comp.first), indexedComparable(comp.second), attrTypeID(comp.compareBy), comp.relation);
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
