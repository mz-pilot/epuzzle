#include "epuzzle/PuzzleSolution.h"

namespace epuzzle
{
    std::ostream& operator<<(std::ostream& os, const PuzzleSolution& data)
    {
        utils::StringTablePrinter tablePrinter{ "  ", "  " };
        tablePrinter.reserve(data.attributes.size());

        for (const auto& attr : data.attributes)
            tablePrinter.addRow(attr.type, attr.values);

        os << tablePrinter;
        return os;
    }

    bool personHasAttribute(const PuzzleSolution& solution, std::string_view person, std::string_view attributeType, std::string_view attributeValue)
    {
        if (solution.attributes.size() >= 1)
        {
            ENSURE(solution.attributes.front().type == PuzzleSolution::personTypeName, "first attribute type in puzzle solution must be personTypeName!");
            const auto& personValues = solution.attributes.front().values;
            const auto itFoundPerson = std::ranges::find(personValues, person);
            if (itFoundPerson != personValues.end())
            {
                const size_t personIndex = itFoundPerson - personValues.begin();
                for (size_t attrIndex = 1; attrIndex < solution.attributes.size(); ++attrIndex)
                {
                    if (solution.attributes[attrIndex].type == attributeType)
                        return solution.attributes[attrIndex].values[personIndex] == attributeValue;
                }
            }
        }
        return false;
    }
}
