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
                m_constraintCheckers.push_back(IConstraint::create(constraint));
            }
        };

        // We'll check constraints from simplest to most complex
        std::ranges::sort(m_constraintCheckers, {}, &IConstraint::complexity);
    }

    bool Validator::isAssignmentValid(AttributeTypeID typeId, AttributeValueID valueId, PersonID personId) const
    {
        for (const auto& property : m_prefilters[typeId])
        {
            if (property.person == personId && ((property.attr.valueId != valueId) != property.negate))
                return false;
        }
        return true;
    }

    bool Validator::isSolutionCandidateValid(const ICandidateIterator& solutionCandidate) const
    {
        // Hot path!
        for (const auto& constraintChecker : m_constraintCheckers)
        {
            if (!constraintChecker->satisfies(solutionCandidate)) [[likely]]
                return false;
        }
        [[unlikely]]
        return true;
    }

}
