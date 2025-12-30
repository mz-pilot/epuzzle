#include "Validator.h"

namespace epuzzle::details::bruteforce
{
    Validator::Validator(size_t attrTypeCount, const std::vector<ConstraintModel>& constraints, bool needPrefiltering)
        : m_prefilters(attrTypeCount)
    {
        // reserve space for all constraints at once, it's not so much
        m_constraintCheckers.reserve(constraints.size());
        for (const auto& constraint : constraints)
        {
            const auto* personProperty = std::get_if<PersonProperty>(&constraint);
            if (needPrefiltering && personProperty)
            {
                m_prefilters[personProperty->attr.typeId].push_back(*personProperty);
            }
            else
            {
                m_constraintCheckers.push_back(ConstraintChecker::create(constraint));
            }
        };

        // We'll check constraints from simplest to most complex
        std::ranges::sort(m_constraintCheckers, {}, &ConstraintChecker::complexity);
    }

    bool Validator::isAssignmentValid(AttributeTypeID attrTypeId, const Assignment& assignment) const
    {
        for (const auto& property : m_prefilters[attrTypeId])
        {
            for (auto valueId = AttributeValueID{ 0 }; valueId < AttributeValueID{ assignment.size() }; ++valueId)
            {
                if (property.person == assignment[valueId] && ((property.attr.valueId != valueId) != property.negate))
                    return false;
            }
        }
        return true;
    }

    bool Validator::isSolutionValid(const SearchSpaceCursor& solutionCandidate) const
    {
        // Hot path!
        for (const auto& constraintChecker : m_constraintCheckers)
        {
            if (!constraintChecker->satisfiedBy(solutionCandidate)) [[likely]]
                return false;
        }
        [[unlikely]]
        return true;
    }

}
