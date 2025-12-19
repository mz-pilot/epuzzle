#include "BruteForceChecker.h"

namespace epuzzle::details
{
    BruteForceChecker::BruteForceChecker(size_t attrTypeCount, const std::vector<IndexedConstraint>& constraintDefs, bool needPrefiltering)
        : m_prefilters(attrTypeCount)
    {
        // reserve space for all constraints at once, it's not so much
        m_constraints.reserve(constraintDefs.size());
        for (const auto& constrDef : constraintDefs)
        {
            const auto* personProperty = std::get_if<PersonProperty>(&constrDef);
            if (needPrefiltering && personProperty)
            {
                m_prefilters[personProperty->attr.typeId].push_back(*personProperty);
            }
            else
            {
                m_constraints.push_back(IConstraint::create(constrDef));
            }
        };

        // We'll check constraints from simplest to most complex
        std::ranges::sort(m_constraints, {}, &IConstraint::complexity);
    }

    bool BruteForceChecker::prefilterCheck(AttributeTypeID typeId, AttributeValueID valueId, PersonID personId) const
    {
        for (const auto& property : m_prefilters[typeId])
        {
            if (property.person == personId && ((property.attr.valueId != valueId) != property.negate))
                return false;
        }
        return true;
    }

    bool BruteForceChecker::mainCheck(const ICandidateIterator& candidate) const
    {
        // Hot path!
        for (const auto& constraint : m_constraints)
        {
            if (!constraint->satisfies(candidate)) [[likely]]
                return false;
        }
        [[unlikely]]
        return true;
    }

}
