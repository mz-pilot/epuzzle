#include "IndexedSolution.h"

namespace epuzzle::details
{
	PuzzleSolution toPuzzleSolution(const IndexedSolution& indexedSolution, const IndexedPuzzleData& data)
	{
		PuzzleSolution puzzleSolution;
		// The first attribute type is "person".
		puzzleSolution.attributes.reserve(1 + data.attrTypeCount());

		const auto personCount = data.personCount();
		std::vector<std::string> persons;
		persons.reserve(personCount);
		for (auto personIndex = 0u; personIndex < personCount; ++personIndex)
		{
			persons.emplace_back(data.personName(PersonID{personIndex}));
		}
		puzzleSolution.attributes.emplace_back(PuzzleData::personTypeName, std::move(persons));

		for (auto typeId = AttributeTypeID{0}; typeId < AttributeTypeID{data.attrTypeCount()}; ++typeId)
		{
			std::vector<std::string> values(personCount);
			for (auto valueId = AttributeValueID{0}; valueId < AttributeValueID{personCount}; ++valueId)
			{
				const auto personId = indexedSolution.attributes[typeId][valueId];
				values[personId.value()] = data.attrValueName(typeId, valueId);
			}
			std::string typeName(data.attrTypeName(typeId));
			puzzleSolution.attributes.emplace_back(std::move(typeName), std::move(values));
		}

		return puzzleSolution;
	}
}