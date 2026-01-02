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
 Также применяется pre-filtering: `AllowFilter` применяется ДО генерации пространства.
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

 Pre-filtering is also used: an `AllowFilter` is applied BEFORE the search space generation.
 If a specific permutation violates a standalone constraint (e.g., "The Norwegian lives
 in the first house"), it is excluded from its respective wheel entirely.
 =============================================================================================
 */

namespace epuzzle::details::bruteforce
{

namespace
{
    // Store set of possible permutations for each attribute type
    // Index: AttributeTypeID -> Value: list of possible permutations of values ​​for this attribute
    using WheelSet = utils::IndexedVector<AttributeTypeID, std::vector<AttributeAssignment>>;

    // -------------------------------- class CursorImpl ------------------------------------------------

    class CursorImpl final : public SearchSpaceCursor
    {
    public:
        CursorImpl(const WheelSet& wheels, std::uint64_t offset, std::uint64_t count)
            : m_wheels(wheels)
            , m_remainingCombinations(count) // parameters checked in SpaceImpl::createCursor
        {
            m_wheelsState.reserve(m_wheels.size());

            // linear index -> multiindex (WheelsState)
            std::uint64_t remainingLinearIndex = offset;
            for (const auto& wheel : m_wheels)
            {
                m_wheelsState.emplace_back(remainingLinearIndex % wheel.size(), wheel.size());
                remainingLinearIndex /= wheel.size();
            }
            ENSURE(remainingLinearIndex == 0, "");
        }

        bool moveNext() override
        {
            // Hot path!

            --m_remainingCombinations;
            // Odometer logic (mileage counter)
            for (auto attrTypeId = AttributeTypeID{ 0 }; m_remainingCombinations && attrTypeId < AttributeTypeID{ m_wheels.size() }; ++attrTypeId)
            {
                // increment the first wheel
                if (++m_wheelsState[attrTypeId].permutIndex < m_wheelsState[attrTypeId].permutCount) [[likely]]
                    return true;

                // if it completes a full rotation -> reset the first wheel and increment the next wheel
                [[unlikely]]
                m_wheelsState[attrTypeId].permutIndex = 0;
            }
            [[unlikely]]
            return false; // last reached
        }

        PersonID ownerOf(AttributeTypeID typeId, AttributeValueID valueId) const override
        {
            return typeId == AttributeTypeID_person ? PersonID{ valueId.value() } : currentAssignment(typeId)[valueId];
        }

        size_t personPosition(PersonID personId, AttributeTypeID typeId) const override
        {
            const auto& assignment = currentAssignment(typeId);
            return std::ranges::find(assignment, personId) - assignment.cbegin(); // it's rare call
        }

        SolutionModel getSolutionModel() const override
        {
            const auto attrTypeCount = m_wheels.size();
            SolutionModel solution{ attrTypeCount };
            for (auto typeId = AttributeTypeID{ 0 }; typeId < AttributeTypeID{ attrTypeCount }; ++typeId)
            {
                solution.setAttributeAssignment(typeId, currentAssignment(typeId));
            }
            return solution;
        }

    private:
        const AttributeAssignment& currentAssignment(AttributeTypeID typeId) const
        {
            return m_wheels[typeId][m_wheelsState[typeId].permutIndex];
        }

    private:
        const WheelSet& m_wheels;

        struct WheelState
        {
            size_t permutIndex = 0;
            size_t permutCount = 0; // cache (hot data)
        };
        utils::IndexedVector<AttributeTypeID, WheelState> m_wheelsState;
        std::uint64_t m_remainingCombinations = 0;
    };

    // -------------------------------- helper functions for class SpaceImpl  --------------------------------------

    // For each attribute generates `wheel` - list of possible permutations of values ​​for this attribute
    // If allowFilter specified - filtering out obviously false ones. It reduces the wheel size.
    WheelSet generateWheelSet(size_t personCount, size_t attrTypeCount, SearchSpace::AllowFilter allowFilter)
    {
        auto generateWheel = [&allowFilter, personCount](AttributeTypeID attrTypeId)
            {
                const bool hasFilter = static_cast<bool>(allowFilter);
                // Generation of permutations:
                // start: [0, 1, 2, 3, 4] -> AttributeAssignment[value_0] = Person_0, AttributeAssignment[value_1] = Person_1, ...
                // next:  [0, 1, 2, 4, 3] ...
                // end:   [4, 3, 2, 1, 0]

                std::vector<AttributeAssignment> permutationList;
                AttributeAssignment attributeAssignment(personCount);
                std::iota(attributeAssignment.begin(), attributeAssignment.end(), PersonID{ 0 }); // init start permutation
                do
                {
                    if (!hasFilter || allowFilter(attrTypeId, attributeAssignment))
                    {
                        permutationList.push_back(attributeAssignment);
                    }
                } while (std::ranges::next_permutation(attributeAssignment).found);

                return permutationList;
            };

        WheelSet wheels;
        wheels.reserve(attrTypeCount);

        for (auto typeId = AttributeTypeID{ 0 }; typeId < AttributeTypeID{ attrTypeCount }; ++typeId)
            wheels.emplace_back(generateWheel(typeId));

        return wheels;
    }

    std::uint64_t calcTotalCombinations(const WheelSet& wheels)
    {
        std::uint64_t totalCombinations = 1;
        for (const auto& wheel : wheels)
        {
            ENSURE(wheel.size() == 0 || totalCombinations <= std::numeric_limits<uint64_t>::max() / wheel.size(),
                "Too much combinations count for using uint64!");
            totalCombinations *= wheel.size();
        }
        return totalCombinations;
    }

    // -------------------------------- class SpaceImpl  ------------------------------------------------

    // Implementation behind the interface - because there can be different implementations (with or without pre-generated permutations, etc.).
    class SpaceImpl final : public SearchSpace
    {
    public:
        explicit SpaceImpl(WheelSet&& wheels)
            : m_wheels(std::move(wheels))
            , m_totalCombinations(calcTotalCombinations(m_wheels))
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

            return std::make_unique<CursorImpl>(m_wheels, offset, count);
        }

    private:
        const WheelSet m_wheels;
        const std::uint64_t m_totalCombinations;
    };

} // namespace

    // -------------------------------- class SearchSpace ------------------------------------------------

    std::unique_ptr<SearchSpace> SearchSpace::create(size_t personCount, size_t attrTypeCount, AllowFilter filter)
    {
        return std::make_unique<SpaceImpl>(generateWheelSet(personCount, attrTypeCount, std::move(filter)));
    }
}
