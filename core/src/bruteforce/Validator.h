#pragma once
#include "IConstraint.h"

namespace epuzzle::details::bruteforce
{
    // Manage checks for bruteforce solving (prefilter and main checks).
    // * If prefiltering enabled: some trivial checks will be performed during the SearchSpace generation stage, 
    //   filtering out obviously unsuitable candidate subsets. These checks will be excluded from the subsequent main check as unnecessary.
    // * If prefiltering disabled: the main check stage will involve a full enumeration and all checks of all possible candidates.
    class Validator
    {
    public:
        Validator(size_t attrTypeCount, const std::vector<ConstraintModel>&, bool needPrefiltering);

        bool isAssignmentValid(AttributeTypeID, AttributeValueID, PersonID) const;
        bool isSolutionCandidateValid(const ICandidateIterator& candidate) const;

    private:
        utils::IndexedVector<AttributeTypeID, std::vector<PersonProperty>> m_prefilters;
        std::vector<std::unique_ptr<IConstraint>> m_constraintCheckers;
    };

}
