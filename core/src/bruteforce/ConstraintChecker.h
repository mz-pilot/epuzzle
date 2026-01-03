#pragma once
#include "SearchSpaceCursor.h"

namespace epuzzle::details::bruteforce
{
    // Checks one type of constraint. Also provides an estimate of the check's complexity (cost).
    class ConstraintChecker
    {
    public:
        static std::unique_ptr<ConstraintChecker> create(const ConstraintModel&);
        virtual ~ConstraintChecker() = default;

        virtual size_t complexity() const = 0;
        virtual bool satisfiedBy(const SearchSpaceCursor& solutionCandidate) const = 0;
    };

}
