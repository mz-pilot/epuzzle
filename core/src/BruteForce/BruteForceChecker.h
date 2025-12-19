#pragma once
#include "IConstraint.h"

namespace epuzzle::details
{
    // Manage checks for brute force solving (prefilter and main checks).
    // * If prefiltering enabled: some trivial checks will be performed during the candidate space generation stage, 
    //   filtering out obviously unsuitable candidate subsets. These checks will be excluded from the subsequent main check as unnecessary.
    // * If prefiltering disabled: the main check stage will involve a full enumeration and all checks of all possible candidates.
    class BruteForceChecker
    {
    public:
        BruteForceChecker(size_t attrTypeCount, const std::vector<IndexedConstraint>&, bool needPrefiltering);

        bool prefilterCheck(AttributeTypeID, AttributeValueID, PersonID) const;
        bool mainCheck(const ICandidateIterator& candidate) const;

    private:
        utils::IndexedVector<AttributeTypeID, std::vector<PersonProperty>> m_prefilters;
        std::vector<std::unique_ptr<IConstraint>> m_constraints;
    };

}
