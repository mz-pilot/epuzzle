#include "epuzzle/Exceptions.h"
#include "epuzzle/PuzzleDefinition.h"

namespace epuzzle
{
    namespace
    {
        using Relation = PuzzleDefinition::Comparison::Relation;
    }

    void normalize(PuzzleDefinition& data)
    {
        for (auto& constr : data.constraints)
        {
            if (auto* fact = std::get_if<PuzzleDefinition::Fact>(&constr))
            {
                if (fact->second.type == PuzzleDefinition::personTypeName && fact->first.type != PuzzleDefinition::personTypeName)
                    std::swap(fact->first, fact->second);
            }
        }
    }

#define ENSURE_VALID_EX(expression, message) \
        ENSURE_SPEC(expression, PuzzleLogicError, "Please, fix the original content of epuzzle::PuzzleDefinition! " << message)

#define ENSURE_VALID(expression) \
        ENSURE_VALID_EX(expression, "")

    void validate(const PuzzleDefinition& data)
    {
        // The dataView copy - view with persons added as one of attributes array. Not so pretty, but very practical for check!
        PuzzleDefinition dataView = data;
        dataView.attributes.emplace_back(PuzzleDefinition::personTypeName, dataView.persons);

        auto validateAllUniqueNonEmpty = []<typename Range, typename Proj = std::identity>(const Range & range, Proj projection = {})
        {
            std::unordered_set<std::string_view> seen;
            seen.reserve(range.size());
            std::ranges::for_each(range, [&seen](const auto& value)
                {
                    ENSURE_VALID(!value.empty());
                    const bool valueUnique = seen.insert(value).second;
                    ENSURE_VALID_EX(valueUnique, "value = " << value);
                },
                projection);
        };

        auto findAttrDescr = [&dataView](const auto& type) -> const PuzzleDefinition::AttributeDescription*
            {
                auto cit = std::ranges::find(dataView.attributes, type, &PuzzleDefinition::AttributeDescription::type);
                return cit != dataView.attributes.cend() ? &(*cit) : nullptr;
            };

        auto validateAttribute = [findAttrDescr](const PuzzleDefinition::Attribute& attr)
            {
                ENSURE_VALID(!attr.type.empty());
                ENSURE_VALID(!attr.value.empty());
                const auto* attrTypeDescr = findAttrDescr(attr.type);
                ENSURE_VALID_EX(attrTypeDescr, "type = " << attr.type);
                const bool valueDescrExist = std::ranges::count(attrTypeDescr->values, attr.value) == 1; // contains() in C++23
                ENSURE_VALID_EX(valueDescrExist, "type = " << attr.type << ", value = " << attr.value);
            };

        // Validate persons

        ENSURE_VALID(dataView.persons.size() >= 2);
        validateAllUniqueNonEmpty(dataView.persons);

        // Validate attributes

        ENSURE_VALID(dataView.attributes.size() >= 2);
        validateAllUniqueNonEmpty(dataView.attributes, &PuzzleDefinition::AttributeDescription::type);
        for (const auto& attr : dataView.attributes)
        {
            ENSURE_VALID(attr.values.size() == dataView.persons.size());
            validateAllUniqueNonEmpty(attr.values);
        }

        // Validate constraints

        ENSURE_VALID(dataView.constraints.size() >= 1);
        for (const auto& constr : dataView.constraints)
        {
            std::visit(utils::overloaded
                {
                    [validateAttribute](const PuzzleDefinition::Fact& fact)
                    {
                    // Validate Facts
                    validateAttribute(fact.first);
                    validateAttribute(fact.second);
                    ENSURE_VALID(fact.first.type != fact.second.type);
                    ENSURE_VALID(fact.second.type != PuzzleDefinition::personTypeName); // see also `normalize(PuzzleDefinition&)` function
                },
                [validateAttribute, findAttrDescr](const PuzzleDefinition::Comparison& comp)
                {
                    // Validate Comparisons
                    validateAttribute(comp.first);
                    validateAttribute(comp.second);
                    ENSURE_VALID_EX(findAttrDescr(comp.compareBy), "compareBy = " << comp.compareBy);
                    ENSURE_VALID(comp.relation != Relation::Uninitialized);
                    ENSURE_VALID(comp.first.type != comp.second.type || comp.first.value != comp.second.value);
                },
                }, constr);
        }
    }

    namespace
    {
        std::ostream& operator<<(std::ostream& os, Relation rel)
        {
            switch (rel)
            {
            case Relation::Uninitialized:   return os << "Uninitialized";
            case Relation::ImmediateLeft:   return os << "ImmediateLeft";
            case Relation::ImmediateRight:  return os << "ImmediateRight";
            case Relation::Adjacent:        return os << "Adjacent";
            case Relation::Before:          return os << "Before";
            case Relation::After:           return os << "After";
            default: ENSURE(false, "Unsupported relation type: (int) " << static_cast<int>(rel));
            };
        }
    }

    std::ostream& operator<<(std::ostream& os, const PuzzleDefinition& data)
    {
        utils::StringTablePrinter tablePrinter{ "  ", "  " };
        tablePrinter.reserve(1 + data.attributes.size());
        tablePrinter.addRow(data.personTypeName, data.persons);
        for (const auto& attr : data.attributes)
            tablePrinter.addRow(attr.type, attr.values);

        os << "persons and attributes:\n";
        os << tablePrinter;
        os << "constraints:\n";
        for (const auto& constr : data.constraints)
        {
            std::visit(utils::overloaded
                {
                    [&os](const PuzzleDefinition::Fact& fact)
                    {
                        os << "  fact:       ";
                        os << "first<" << fact.first.type << ":" << fact.first.value;
                        os << ">, second<" << fact.second.type << (fact.secondNegate ? ": NOT " : ":") << fact.second.value << ">\n";
                    },
                    [&os](const PuzzleDefinition::Comparison& comp)
                    {
                        os << "  comparison: ";
                        os << "first<" << comp.first.type << ":" << comp.first.value;
                        os << ">, compare_by<" << comp.compareBy;
                        os << ">, relation<" << comp.relation;
                        os << ">, second<" << comp.second.type << ":" << comp.second.value << ">\n";
                    },
                }, constr);
        }

        return os;
    }

}
