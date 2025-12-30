#pragma once
#include "ConstraintChecker.h"

namespace epuzzle::details::bruteforce
{
    // Manage checks for bruteforce solving (prefilter and main checks).
    // * If prefiltering enabled: some trivial checks will be performed during the SearchSpace generation stage, 
    //   filtering out obviously unsuitable combinations subsets. These checks will be excluded from the subsequent main check as unnecessary.
    // * If prefiltering disabled: the main check stage will involve a full enumeration and all checks of all possible combinations.
    class Validator
    {
    public:
        Validator(size_t attrTypeCount, const std::vector<ConstraintModel>&, bool needPrefiltering);

        bool isAttributeAssignmentValid(AttributeTypeID, const AttributeAssignment&) const;

        bool isSolutionValid(const SearchSpaceCursor&) const;

    private:
        utils::IndexedVector<AttributeTypeID, std::vector<PersonProperty>> m_prefilters;
        std::vector<std::unique_ptr<ConstraintChecker>> m_constraintCheckers;
    };

}
