#pragma once
#include "SearchSpaceCursor.h"

namespace epuzzle::details::bruteforce
{
    // Search space for bruteforce. Contain all possible combinations of attribute values assigned to persons.
    // Interface - because there can be different implementations (with or without pre-generated permutations, etc.).
    class SearchSpace
    {
    public:
        // AllowFilter can exclude the assignment of obviously invalid attribute values ​​(in this case, return false).
        using AllowFilter = std::function<bool(AttributeTypeID, const Assignment&)>;

        static std::unique_ptr<SearchSpace> create(size_t personCount, size_t attrTypeCount, AllowFilter);
        virtual ~SearchSpace() = default;

        virtual std::uint64_t totalCombinations() const = 0;

        // Cursor range: offset - initial position, count - cursor range length
        [[nodiscard]] virtual std::unique_ptr<SearchSpaceCursor> createCursor(std::uint64_t offset, std::uint64_t count) const = 0;

        [[nodiscard]] std::unique_ptr<SearchSpaceCursor> createCursor() const { return createCursor(0, totalCombinations()); }
    };
}
