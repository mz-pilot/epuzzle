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
    using Odometer = utils::IndexedVector<AttributeTypeID, std::vector<AttributeAssignment>>;

    // -------------------------------- class CursorImpl ------------------------------------------------

    class CursorImpl final : public SearchSpaceCursor
    {
    public:
        CursorImpl(const Odometer& odometer, std::uint64_t offset, std::uint64_t count)
            : m_odometer(odometer)
            , m_remainingCombinations(count) // parameters checked in SpaceImpl::createCursor
        {
            m_odometerState.reserve(m_odometer.size());

            // Set begining odometer state to `offset` position: convert the linear index (offset) to m_odometerState.
            // This is the same as calling MoveNext() `offset`-times, starting from the beginning.
            std::uint64_t remainingLinearIndex = offset;
            for (const auto& wheel : m_odometer)
            {
                // note: when the pre-filter enabled, each wheel may be a different size (permutations count)
                m_odometerState.emplace_back(wheel.size(), remainingLinearIndex % wheel.size());
                remainingLinearIndex /= wheel.size();
            }
            ENSURE(remainingLinearIndex == 0, "");
        }

        bool moveNext() override
        {
            // Hot path!

            if (--m_remainingCombinations == 0) [[unlikely]]
                return false;

            // Odometer logic (mileage counter)
            for (auto& wheelState : m_odometerState)
            {
                // increment the first wheel
                if (++wheelState.position < wheelState.size) [[likely]]
                    return true;

                [[unlikely]]
                // if it completes a full rotation -> reset the first wheel and increment the next wheel
                wheelState.position = 0;
            }

            [[unlikely]]
            return false; // last reached
        }

        PersonID ownerOf(AttributeTypeID typeId, AttributeValueID valueId) const override
        {
            return currentAssignment(typeId)[valueId];
        }

        size_t personPosition(PersonID personId, AttributeTypeID typeId) const override
        {
            const auto& assignment = currentAssignment(typeId);
            return std::ranges::find(assignment, personId) - assignment.cbegin(); // it's rare call
        }

        SolutionModel getSolutionModel() const override
        {
            const auto attrTypeCount = m_odometer.size();
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
            return m_odometer[typeId][m_odometerState[typeId].position];
        }

    private:
        const Odometer& m_odometer;

        struct WheelState
        {
            const size_t size; // cache (hot data)
            size_t position = 0;
        };
        utils::IndexedVector<AttributeTypeID, WheelState> m_odometerState;
        std::uint64_t m_remainingCombinations = 0;
    };

    // -------------------------------- helper functions for class SpaceImpl  --------------------------------------

    // For each attribute generates `wheel` - list of possible permutations of values ​​for this attribute
    // If allowFilter specified - filtering out obviously false ones. It reduces the wheel size.
    Odometer generateOdometer(size_t personCount, size_t attrTypeCount, SearchSpace::AllowFilter allowFilter)
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

        Odometer odometer(attrTypeCount);
        for (auto attrTypeId = AttributeTypeID{ 0 }; attrTypeId < AttributeTypeID{ attrTypeCount }; ++attrTypeId)
            odometer[attrTypeId] = generateWheel(attrTypeId);

        return odometer;
    }

    std::uint64_t calcTotalCombinations(const Odometer& odometer)
    {
        std::uint64_t totalCombinations = 1;
        for (const auto& wheel : odometer)
        {
            ENSURE(wheel.size() == 0 || totalCombinations <= std::numeric_limits<uint64_t>::max() / wheel.size(),
                "Too much combinations count for using uint64!");
            totalCombinations *= wheel.size();
        }
        return totalCombinations;
    }

    // -------------------------------- class SpaceImpl  ------------------------------------------------

    // Implementation behind the interface - because there can be different implementations (with or without pre-generated permutations, etc.).
    // All class is const - ready for multithreaded use and cache friendly.
    class SpaceImpl final : public SearchSpace
    {
    public:
        explicit SpaceImpl(Odometer&& odometer)
            : m_odometer(std::move(odometer))
            , m_totalCombinations(calcTotalCombinations(m_odometer))
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

            return std::make_unique<CursorImpl>(m_odometer, offset, count);
        }

    private:
        const Odometer m_odometer;
        const std::uint64_t m_totalCombinations;
    };

} // namespace

    // -------------------------------- class SearchSpace ------------------------------------------------

    std::unique_ptr<SearchSpace> SearchSpace::create(size_t personCount, size_t attrTypeCount, AllowFilter filter)
    {
        return std::make_unique<SpaceImpl>(generateOdometer(personCount, attrTypeCount, std::move(filter)));
    }
}
