#pragma once
#include "ICandidateIterator.h"

namespace epuzzle::details
{
    // Space of potential solution candidates
    class ICandidatesSpace
    {
    public:
        // AllowFilter can exclude obviously invalid combinations of parameters (should return false in that case)
        using AllowFilter = std::function<bool(AttributeTypeID, AttributeValueID, PersonID)>;

        static std::unique_ptr<ICandidatesSpace> create(size_t personCount, size_t attrTypeCount, AllowFilter);
        virtual ~ICandidatesSpace() = default;

        virtual std::uint64_t candidatesCount() const = 0;
        // empty unique_ptr if no candidates
        [[nodiscard]] virtual std::unique_ptr<ICandidateIterator> createIterator(std::uint64_t firstCandidate, std::uint64_t count) const = 0;
    };
}
