#include "IndexedSolution.h"

namespace epuzzle::details
{
    PuzzleSolution toPuzzleSolution(const IndexedSolution& indexedSolution, const PuzzleModel& puzzleModel)
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
        puzzleSolution.attributes.emplace_back(PuzzleDefinition::personTypeName, std::move(persons));

        for (auto typeId = AttributeTypeID{ 0 }; typeId < AttributeTypeID{ puzzleModel.attrTypeCount() }; ++typeId)
        {
            std::vector<std::string> values(personCount);
            for (auto valueId = AttributeValueID{ 0 }; valueId < AttributeValueID{ personCount }; ++valueId)
            {
                const auto personId = indexedSolution.attributes[typeId][valueId];
                values[personId.value()] = puzzleModel.attrValueName(typeId, valueId);
            }
            std::string typeName(puzzleModel.attrTypeName(typeId));
            puzzleSolution.attributes.emplace_back(std::move(typeName), std::move(values));
        }

        return puzzleSolution;
    }
}
