#pragma once
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace epuzzle
{
    // The puzzle solution table: attribute type x list of values. The first attribute type is `person` (value of personTypeName). 
    // The values of other attributes with the same index belong to the same person.
    // Example: "person"      - "Spaniard", "Englishman"
    //          "house_color" - "green",    "white"
    struct PuzzleSolution
    {
        static constexpr auto personTypeName = "person";

        struct Attribute
        {
            std::string type; // example: "house_color"
            std::vector<std::string> values; // example: "green"
        };

        std::vector<Attribute> attributes; // table
    };

    std::ostream& operator<<(std::ostream&, const PuzzleSolution&);

    bool personHasAttribute(const PuzzleSolution& solution, std::string_view person, std::string_view attributeType, std::string_view attributeValue);
}
