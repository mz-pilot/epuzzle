#include "IConstraint.h"

namespace epuzzle::details
{
namespace
{
    template <typename TParams>
    class ConstraintImpl final : public IConstraint
    {
    public:
        explicit ConstraintImpl(const TParams& params)
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

        bool satisfies(const ICandidateIterator& candidate) const override
        {
            if constexpr (std::is_same_v<TParams, PersonProperty>)
            {
                return (m_params.person == candidate.ownerOf(m_params.attr.typeId, m_params.attr.valueId))
                    != m_params.negate;
            }
            else if constexpr (std::is_same_v<TParams, SameOwner>)
            {
                return (candidate.ownerOf(m_params.first.typeId, m_params.first.valueId) == candidate.ownerOf(m_params.second.typeId, m_params.second.valueId))
                    != m_params.secondNegate;
            }
            else if constexpr (std::is_same_v<TParams, PositionComparison>)
            {
                const auto owner1 = candidate.ownerOf(m_params.first.typeId, m_params.first.valueId);
                const auto owner2 = candidate.ownerOf(m_params.second.typeId, m_params.second.valueId);
                const size_t pos1 = candidate.personPosition(owner1, m_params.compareByType);
                const size_t pos2 = candidate.personPosition(owner2, m_params.compareByType);

                using Relation = PuzzleData::Comparison::Relation;
                switch (m_params.relation)
                {
                case Relation::ImmediateLeft:   return pos1 < pos2 && (pos2 - pos1 == 1);
                case Relation::ImmediateRight:  return pos1 > pos2 && (pos1 - pos2 == 1);
                case Relation::Adjacent:        return std::max(pos1, pos2) - std::min(pos1, pos2) == 1;
                case Relation::Before:          return pos1 < pos2;
                case Relation::After:           return pos1 > pos2;
                default: ENSURE(false, "Unexpected PuzzleData::ComparisonParams::Relation: (int)" << static_cast<int>(m_params.relation));
                };
            }
            else
                static_assert(false, "Need special implementation in satisfies() method for current TParams!");
        }

    private:
        const TParams m_params;
    };

} // namespace

    std::unique_ptr<IConstraint> IConstraint::create(const IndexedConstraint& constrDef)
    {
        return std::visit(
            [](const auto& arg) -> std::unique_ptr<IConstraint>
            {
                using T = std::decay_t<decltype(arg)>;
                return std::make_unique<ConstraintImpl<T>>(arg);
            },
            constrDef);
    }

}
