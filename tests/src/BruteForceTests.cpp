#include "BruteForce/SpaceParallelDistributor.h"

namespace epuzzle::tests
{

	namespace { using namespace epuzzle::details; }

	TEST(BruteForceTests, ICandidatesSpaceCreateOk)
	{
		constexpr size_t personCount = 6, attrCount = 6;
		std::unique_ptr<ICandidatesSpace> space;
		ASSERT_NO_THROW(space = ICandidatesSpace::create(personCount, attrCount, {}));
		ASSERT_TRUE(space);
		using namespace utils;
		EXPECT_EQ(space->candidatesCount(), power(factorial(personCount), attrCount));
	}

	TEST(BruteForceTests, IterateAllWithDistributorOk)
	{
		constexpr size_t personCount = 5, attrCount = 4;
		std::unique_ptr<ICandidatesSpace> space;
		ASSERT_NO_THROW(space = ICandidatesSpace::create(personCount, attrCount, {}));
		ASSERT_TRUE(space);
		const auto totalCandidates = space->candidatesCount();
		SpaceParallelDistributor distributor{ totalCandidates };

		std::uint64_t iterated = 0;
		while (auto iter = distributor.take(*space))
		{
			do
				++iterated;
			while (iter->next());
		}
		EXPECT_EQ(iterated, totalCandidates);
	}

}