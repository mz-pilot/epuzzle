#include "bruteforce/SearchSpace.h"
#include "bruteforce/SpaceSplitter.h"

namespace epuzzle::tests
{

    namespace { using namespace epuzzle::details::bruteforce; }

    TEST(BruteForceTests, SearchSpaceCreateOk)
    {
        constexpr size_t personCount = 6, attrCount = 6;
        std::unique_ptr<SearchSpace> space;
        ASSERT_NO_THROW(space = SearchSpace::create(personCount, attrCount, {}));
        ASSERT_TRUE(space);
        using namespace utils;
        EXPECT_EQ(space->totalCombinations(), power(factorial(personCount), attrCount));
    }

    TEST(BruteForceTests, IterateAllWithSplitterOk)
    {
        constexpr size_t personCount = 5, attrCount = 4;
        std::unique_ptr<SearchSpace> space;
        ASSERT_NO_THROW(space = SearchSpace::create(personCount, attrCount, {}));
        ASSERT_TRUE(space);
        const auto totalCombinations = space->totalCombinations();
        SpaceSplitter splitter{ totalCombinations };

        std::uint64_t iterated = 0;
        std::unique_ptr<SearchSpaceCursor> cursor;
        while (auto chunk = splitter.nextChunk())
        {
            ASSERT_TRUE(cursor = space->createCursor(chunk->offset, chunk->count));
            do
                ++iterated;
            while (cursor->next());
        }
        EXPECT_EQ(iterated, totalCombinations);
    }

}
