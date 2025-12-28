#pragma once
#include "SearchSpaceCursor.h"

namespace epuzzle::details::bruteforce
{
    // Space of potential solution candidates. Interface - because may be different implementations (with/without prefetch, etc).
    class SearchSpace
    {
    public:
        // AllowFilter can exclude obviously invalid combinations of parameters (should return false in that case)
        using AllowFilter = std::function<bool(AttributeTypeID, AttributeValueID, PersonID)>;

        static std::unique_ptr<SearchSpace> create(size_t personCount, size_t attrTypeCount, AllowFilter);
        virtual ~SearchSpace() = default;

        virtual std::uint64_t totalCandidates() const = 0;
        // empty unique_ptr if no candidates
        [[nodiscard]] virtual std::unique_ptr<SearchSpaceCursor> createCursor(std::uint64_t offset, std::uint64_t count) const = 0;
        [[nodiscard]] std::unique_ptr<SearchSpaceCursor> createCursor() const { return createCursor(0, totalCandidates()); }
    };
}
