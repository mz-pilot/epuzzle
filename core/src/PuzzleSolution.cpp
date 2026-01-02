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
        // So, we have 'special attribute' - 'person' and at least 2 attributes with values
        const auto minAttributesCount = 1 + 2;
        ENSURE(solution.attributes.size() >= minAttributesCount && solution.attributes.front().type == PuzzleSolution::personTypeName,
            "PuzzleSolution has unexpected format!");

        const auto& personValues = solution.attributes.front().values;
        const auto itFoundPerson = std::ranges::find(personValues, person);
        if (itFoundPerson != personValues.end())
        {
            const size_t personIndex = itFoundPerson - personValues.begin();
            for (size_t i = 1; i < solution.attributes.size(); ++i)
            {
                if (solution.attributes[i].type == attributeType)
                    return solution.attributes[i].values[personIndex] == attributeValue;
            }
        }
        return false;
    }
}
