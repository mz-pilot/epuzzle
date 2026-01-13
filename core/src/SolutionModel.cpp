#include "SolutionModel.h"

namespace epuzzle::details
{
    static_assert(std::string_view(PuzzleSolution::personTypeName) == std::string_view(PuzzleDefinition::personTypeName));

    SolutionModel::SolutionModel(size_t attrTypeCount)
        : m_attributes(attrTypeCount)
    {
    }

    void SolutionModel::setAttributeAssignment(AttributeTypeID attrTypeId, AttributeAssignment assignment)
    {
        ENSURE(attrTypeId.value() < m_attributes.size(), "");
        m_attributes[attrTypeId] = std::move(assignment);
    }

    PuzzleSolution SolutionModel::toPuzzleSolution(const PuzzleModel& puzzleModel) const
    {
        PuzzleSolution puzzleSolution;
        // The first attribute type is "person".
        puzzleSolution.attributes.reserve(1 + puzzleModel.attrTypeCount());

        const auto personCount = puzzleModel.personCount();
        std::vector<std::string> persons;
        persons.reserve(personCount);
        for (auto personIndex = 0u; personIndex < personCount; ++personIndex)
        {
            persons.emplace_back(puzzleModel.personName(PersonID{ personIndex }));
        }
        puzzleSolution.attributes.emplace_back(PuzzleSolution::personTypeName, std::move(persons));

        for (auto typeId = AttributeTypeID{ 0 }; typeId < AttributeTypeID{ puzzleModel.attrTypeCount() }; ++typeId)
        {
            std::vector<std::string> values(personCount);
            for (auto valueId = AttributeValueID{ 0 }; valueId < AttributeValueID{ personCount }; ++valueId)
            {
                const auto personId = m_attributes[typeId][valueId];
                values[personId.value()] = puzzleModel.attrValueName(typeId, valueId);
            }
            std::string typeName(puzzleModel.attrTypeName(typeId));
            puzzleSolution.attributes.emplace_back(std::move(typeName), std::move(values));
        }

        return puzzleSolution;
    }
}
