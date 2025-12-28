#pragma once
#include "SearchSpaceCursor.h"

namespace epuzzle::details::bruteforce
{
    // Constraint, that can be checked. Also it offers calculation complexity (cost of check).
    class IConstraint
    {
    public:
        static std::unique_ptr<IConstraint> create(const ConstraintModel&);
        virtual ~IConstraint() = default;

        virtual size_t complexity() const = 0;
        virtual bool satisfies(const SearchSpaceCursor& solutionCandidate) const = 0;
    };

}
