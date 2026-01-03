#include "ConstraintChecker.h"

namespace epuzzle::details::bruteforce
{
namespace
{
    // No big `switch` here but `if constexpr ... else static_assert` - so that compiler can help us add new constraints correctly
    template <typename TParams>
    class CheckerImpl final : public ConstraintChecker
    {
    public:
        explicit CheckerImpl(const TParams& params)
            : m_params(params)
        {
        }

        size_t complexity() const override
        {
            if		constexpr (std::is_same_v<TParams, PersonProperty>)     return 1;
            else if constexpr (std::is_same_v<TParams, SameOwner>)          return 2;
            else if constexpr (std::is_same_v<TParams, PositionComparison>) return 4;
            else static_assert(false, "Need return complexity value for current TParams!");
        }

        bool satisfiedBy(const SearchSpaceCursor& solutionCandidate) const override
        {
            if constexpr (std::is_same_v<TParams, PersonProperty>)
            {
                return (m_params.person == solutionCandidate.ownerOf(m_params.attr.typeId, m_params.attr.valueId))
                    != m_params.negate;
            }
            else if constexpr (std::is_same_v<TParams, SameOwner>)
            {
                return (solutionCandidate.ownerOf(m_params.first.typeId, m_params.first.valueId) == solutionCandidate.ownerOf(m_params.second.typeId, m_params.second.valueId))
                    != m_params.secondNegate;
            }
            else if constexpr (std::is_same_v<TParams, PositionComparison>)
            {
                auto getPersonByVariant = [&solutionCandidate](const std::variant<PersonID, Attribute>& personVariant) -> PersonID
                    {
                        if (const auto* personId = std::get_if<PersonID>(&personVariant))
                            return *personId;
                        else if (const auto* attribute = std::get_if<Attribute>(&personVariant))
                            return solutionCandidate.ownerOf(attribute->typeId, attribute->valueId);
                        else
                            ENSURE(false, "Need special handler for specified personVariant type!");
                        return PersonID{};
                    };

                const size_t pos1 = solutionCandidate.personPosition(getPersonByVariant(m_params.first), m_params.compareByType);
                const size_t pos2 = solutionCandidate.personPosition(getPersonByVariant(m_params.second), m_params.compareByType);

                using Relation = decltype(m_params.relation);
                switch (m_params.relation)
                {
                case Relation::ImmediateLeft:   return pos1 < pos2 && (pos2 - pos1 == 1);
                case Relation::ImmediateRight:  return pos1 > pos2 && (pos1 - pos2 == 1);
                case Relation::Adjacent:        return std::max(pos1, pos2) - std::min(pos1, pos2) == 1;
                case Relation::Before:          return pos1 < pos2;
                case Relation::After:           return pos1 > pos2;
                default: ENSURE(false, "Unexpected PositionComparison::relation: (int)" << static_cast<int>(m_params.relation));
                };
            }
            else
                static_assert(false, "Need special implementation in satisfiedBy() method for current TParams!");
        }

    private:
        const TParams m_params;
    };

} // namespace

    std::unique_ptr<ConstraintChecker> ConstraintChecker::create(const ConstraintModel& constraint)
    {
        return std::visit(
            [](const auto& arg) -> std::unique_ptr<ConstraintChecker>
            {
                using T = std::decay_t<decltype(arg)>;
                return std::make_unique<CheckerImpl<T>>(arg);
            },
            constraint);
    }

}
