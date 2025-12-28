#include "epuzzle/Exceptions.h"
#include "SearchSpace.h"

namespace epuzzle::details::bruteforce
{

namespace
{
    using AttributeSpace = utils::IndexedVector<AttributeTypeID, std::vector<Assignment>>; // All attributes actual permutations

    // -------------------------------- class IteratorImpl ------------------------------------------------

    class IteratorImpl final : public ICandidateIterator
    {
    public:
        IteratorImpl(const AttributeSpace& attrs, std::uint64_t firstCandidate, std::uint64_t count)
            : m_attributes(attrs)
            , m_remainingCandidates(count) // parameters checked in CandidatesSpaceImpl::createIterator
        {
            m_context.reserve(m_attributes.size());

            // linear index -> multiindex
            std::uint64_t remainingLinearIndex = firstCandidate;
            for (const auto& attr : m_attributes)
            {
                const auto attrPermutationsCount = attr.size();
                const std::uint64_t radix = attrPermutationsCount;
                m_context.emplace_back(remainingLinearIndex % radix, radix);
                remainingLinearIndex /= radix;
            }
            ENSURE(remainingLinearIndex == 0, "");
        }

        bool next() override
        {
            // Hot path! 
            --m_remainingCandidates;
            for (auto attrTypeId = AttributeTypeID{ 0 }; attrTypeId < AttributeTypeID{ m_attributes.size() } && m_remainingCandidates; ++attrTypeId)
            {
                if (++m_context[attrTypeId].permutIndex < m_context[attrTypeId].permutCount) [[likely]]
                    return true;

                [[unlikely]]
                m_context[attrTypeId].permutIndex = 0;
            }
            [[unlikely]]
            return false; // last reached
        }

        PersonID ownerOf(AttributeTypeID typeId, AttributeValueID valueId) const override
        {
            return typeId == AttributeTypeID_person ? PersonID{ valueId.value() } : currentAttrAssignment(typeId)[valueId];
        }

        size_t personPosition(PersonID personId, AttributeTypeID typeId) const override
        {
            const auto& assignment = currentAttrAssignment(typeId);
            return std::ranges::find(assignment, personId) - assignment.cbegin();
        }

        SolutionModel getSolutionModel() const override
        {
            SolutionModel solution;
            solution.attributes.reserve(m_attributes.size());

            for (auto typeId = AttributeTypeID{ 0 }; typeId < AttributeTypeID{ m_attributes.size() }; ++typeId)
            {
                solution.attributes.push_back(currentAttrAssignment(typeId));
            }
            return solution;
        }

    private:
        const Assignment& currentAttrAssignment(AttributeTypeID typeId) const
        {
            return m_attributes[typeId][m_context[typeId].permutIndex];
        }

    private:
        const AttributeSpace& m_attributes;

        struct AttributeContext
        {
            size_t permutIndex = 0;
            size_t permutCount = 0; // cache (hot data)
        };
        utils::IndexedVector<AttributeTypeID, AttributeContext> m_context;
        std::uint64_t m_remainingCandidates = 0;
    };

    // -------------------------------- helper functions for class CandidatesSpaceImpl  --------------------------------------

    AttributeSpace makeAttributeSpace(size_t personCount, size_t attrTypeCount, SearchSpace::AllowFilter filter)
    {
        auto makeAttrPermutations = [&filter, personCount](AttributeTypeID typeId)
            {
                auto allowFilter = [&filter, typeId](const Assignment& assignment) -> bool
                    {
                        for (auto valueId = AttributeValueID{ 0 }; valueId < AttributeValueID{ assignment.size() }; ++valueId)
                        {
                            if (!filter(typeId, valueId, assignment[valueId]))
                                return false;
                        }
                        return true;
                    };

                const bool hasFilter = static_cast<bool>(filter);
                std::vector<Assignment> permutations;
                Assignment assignment(personCount);
                std::iota(assignment.begin(), assignment.end(), PersonID{ 0 });
                do
                {
                    if (!hasFilter || allowFilter(assignment))
                    {
                        permutations.push_back(assignment);
                    }
                } while (std::ranges::next_permutation(assignment).found);

                return permutations;
            };

        AttributeSpace attrs;
        attrs.reserve(attrTypeCount);

        for (auto typeId = AttributeTypeID{ 0 }; typeId < AttributeTypeID{ attrTypeCount }; ++typeId)
            attrs.emplace_back(makeAttrPermutations(typeId));

        return attrs;
    }

    std::uint64_t calcTotalCandidates(const AttributeSpace& attrs)
    {
        std::uint64_t totalCandidates = 1;
        for (const auto& attr : attrs)
        {
            const auto attrPermutationCount = attr.size();
            ENSURE(attrPermutationCount == 0 || totalCandidates <= std::numeric_limits<uint64_t>::max() / attrPermutationCount,
                "Too much candidates count for using uint64!");
            totalCandidates *= attrPermutationCount;
        }
        return totalCandidates;
    }

    // -------------------------------- class CandidatesSpaceImpl  ------------------------------------------------

    class CandidatesSpaceImpl final : public SearchSpace
    {
    public:
        explicit CandidatesSpaceImpl(AttributeSpace&& attributes)
            : m_attributes(std::move(attributes))
            , m_totalCandidates(calcTotalCandidates(m_attributes))
        {
        }

        std::uint64_t candidatesCount() const override
        {
            return m_totalCandidates;
        }

        std::unique_ptr<ICandidateIterator> createIterator(std::uint64_t firstCandidate, std::uint64_t count) const override
        {
            if (m_totalCandidates == 0 || count == 0 || m_totalCandidates < firstCandidate || m_totalCandidates - firstCandidate < count)
                return {};

            return std::make_unique<IteratorImpl>(m_attributes, firstCandidate, count);
        }

    private:
        const AttributeSpace m_attributes;
        const std::uint64_t m_totalCandidates;
    };

} // namespace

        // -------------------------------- class SearchSpace ------------------------------------------------

    std::unique_ptr<SearchSpace> SearchSpace::create(size_t personCount, size_t attrTypeCount, AllowFilter filter)
    {
        return std::make_unique<CandidatesSpaceImpl>(makeAttributeSpace(personCount, attrTypeCount, std::move(filter)));
    }
}
