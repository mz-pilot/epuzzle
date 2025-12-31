#include "epuzzle/Exceptions.h"
#include "SearchSpace.h"

/*
 ======================================================================================
 ARCHITECTURE NOTE: SEARCH SPACE GENERATION
 =============================================================================================
     CONCEPT
 Представьте кодовый замок с N колёсиками. На каждом колёсике нарисованы не цифры 0-9,
 а уникальные варианты перестановки конкретного атрибута (например, Цвета Домов).
  1. Мы генерируем все валидные перестановки для каждого типа атрибута отдельно.
     Например, если у нас 5 домов и 5 цветов -> это 5! = 120 вариантов раскраски.
     Это наши "числа" на колесике.
  2. Пространство поиска — перебор всех комбинаций положений наших "колёсиков".
  3. Итерация работает как одометр (счетчик пробега) в машине:
     - Сдвигаем первое колёсико.
     - Если оно сделало полный оборот -> сдвигаем второе колёсико, а первое сбрасываем.
     - И так далее до последнего атрибута.
 Также применяется prefiltering: `AllowFilter` применяется ДО генерации пространства.
 Если перестановка нарушает условие (например, "Норвежец живет в доме #1"), она даже
 не попадает в список допустимых значений для "колёсика".
 ---------------------------------------------------------------------------------------------
     CONCEPT
 Imagine a combination lock with N wheels. Instead of digits 0–9, each wheel
 contains all unique permutations of a specific attribute type (e.g., House Colors).
  1. We generate all valid permutations for each attribute type independently.
     Example: 5 houses and 5 colors result in 5! = 120 possible coloring patterns.
     These patterns represent the "digits" on a single wheel.
  2. The search space is defined as the Cartesian product of all wheels, representing
     every possible combination of attribute assignments.
  3. The iterator functions like a vehicle odometer (mileage counter):
     - Increment the first wheel.
     - If it completes a full rotation -> increment the second wheel and reset the first.
     - This continues until the last attribute is reached.

 Prefiltering is also used: an `AllowFilter` is applied BEFORE the search space is finalized.
 If a specific permutation violates a standalone constraint (e.g., "The Norwegian lives
 in the first house"), it is excluded from its respective wheel entirely.
 =============================================================================================
 */

namespace epuzzle::details::bruteforce
{

namespace
{
    using AttributeSpace = utils::IndexedVector<AttributeTypeID, std::vector<AttributeAssignment>>; // All attributes actual permutations

    // -------------------------------- class CursorImpl ------------------------------------------------

    class CursorImpl final : public SearchSpaceCursor
    {
    public:
        CursorImpl(const AttributeSpace& attrs, std::uint64_t firstCandidate, std::uint64_t count)
            : m_attributes(attrs)
            , m_remainingCandidates(count) // parameters checked in SpaceImpl::createCursor
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
            SolutionModel solution{ m_attributes.size() };
            for (auto typeId = AttributeTypeID{ 0 }; typeId < AttributeTypeID{ m_attributes.size() }; ++typeId)
            {
                solution.setAttributeAssignment(typeId, currentAttrAssignment(typeId));
            }
            return solution;
        }

    private:
        const AttributeAssignment& currentAttrAssignment(AttributeTypeID typeId) const
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

    // -------------------------------- helper functions for class SpaceImpl  --------------------------------------

    AttributeSpace makeAttributeSpace(size_t personCount, size_t attrTypeCount, SearchSpace::AllowFilter allowFilter)
    {
        auto makeAttrPermutations = [&allowFilter, personCount](AttributeTypeID attrTypeId)
            {
                const bool hasFilter = static_cast<bool>(allowFilter);
                std::vector<AttributeAssignment> permutations;
                AttributeAssignment assignment(personCount);
                std::iota(assignment.begin(), assignment.end(), PersonID{ 0 });
                do
                {
                    if (!hasFilter || allowFilter(attrTypeId, assignment))
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

    std::uint64_t calcTotalCombinations(const AttributeSpace& attrs)
    {
        std::uint64_t totalCombinations = 1;
        for (const auto& attr : attrs)
        {
            const auto attrPermutationCount = attr.size();
            ENSURE(attrPermutationCount == 0 || totalCombinations <= std::numeric_limits<uint64_t>::max() / attrPermutationCount,
                "Too much combinations count for using uint64!");
            totalCombinations *= attrPermutationCount;
        }
        return totalCombinations;
    }

    // -------------------------------- class SpaceImpl  ------------------------------------------------

    // Implementation behind the interface - because there can be different implementations (with or without pre-generated permutations, etc.).
    class SpaceImpl final : public SearchSpace
    {
    public:
        explicit SpaceImpl(AttributeSpace&& attributes)
            : m_attributes(std::move(attributes))
            , m_totalCombinations(calcTotalCombinations(m_attributes))
        {
        }

        std::uint64_t totalCombinations() const override
        {
            return m_totalCombinations;
        }

        std::unique_ptr<SearchSpaceCursor> createCursor(std::uint64_t offset, std::uint64_t count) const override
        {
            if (m_totalCombinations == 0 || count == 0 || offset >= m_totalCombinations || count > m_totalCombinations - offset)
                return {};

            return std::make_unique<CursorImpl>(m_attributes, offset, count);
        }

    private:
        const AttributeSpace m_attributes;
        const std::uint64_t m_totalCombinations;
    };

} // namespace

        // -------------------------------- class SearchSpace ------------------------------------------------

    std::unique_ptr<SearchSpace> SearchSpace::create(size_t personCount, size_t attrTypeCount, AllowFilter filter)
    {
        return std::make_unique<SpaceImpl>(makeAttributeSpace(personCount, attrTypeCount, std::move(filter)));
    }
}
