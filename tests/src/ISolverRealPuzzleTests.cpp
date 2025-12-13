#include "epuzzle/ISolver.h"
#include "epuzzle/PuzzleDataLoader.h"
#include "TestUtils.h"

namespace epuzzle::tests
{
	
	class ISolverRealPuzzleTests : public testing::TestWithParam<SolverConfig>
	{
	public:
	protected:
		void SetUp() override 
		{
			if (GetParam().solvingMethod == SolverConfig::SolvingMethod::Reasoning)
				GTEST_SKIP() << "Reasoner implementation coming in next version!";
		}

		auto solve(std::string_view fileName)
		{
			std::unique_ptr<ISolver> solver;
			std::vector<PuzzleSolution> solutions;
			EXPECT_NO_THROW(solver = ISolver::create(GetParam(), puzzleDataFromFile(test_utils::puzzlePath(fileName))));
			if (solver)
			{
				EXPECT_NO_THROW(solutions = solver->solve({}));
			}
			return solutions;
		}
	};

	namespace 
	{
		using BFConfig = SolverConfig::BruteForce;
		using Method = SolverConfig::SolvingMethod;
		using ExecPolicy = SolverConfig::BruteForce::ExecPolicy;
	}

	TEST_P(ISolverRealPuzzleTests, DSSolutionCount128)
	{
		const auto solutions = solve("puzzle_gen_by_ds_r1.toml");
		ASSERT_EQ(solutions.size(), 128);
	}

	TEST_P(ISolverRealPuzzleTests, EinsteinsSolutionCount1Correct)
	{
		const auto solutions = solve("puzzle_einsteins.toml");
		ASSERT_EQ(solutions.size(), 1);
		EXPECT_TRUE(personHasAttribute(solutions.front(), "Норвежец", "beverage", "вода"));
		EXPECT_TRUE(personHasAttribute(solutions.front(), "Японец", "pet", "зебра"));
	}

	TEST_P(ISolverRealPuzzleTests, SolveCanceled)
	{
		std::unique_ptr<ISolver> solver;
		// Use hard puzzle with long compute (weeks)
		ASSERT_NO_THROW(solver = ISolver::create(GetParam(), puzzleDataFromFile(test_utils::puzzlePath("puzzle_six_houses.toml"))));
		ASSERT_TRUE(static_cast<bool>(solver));
		using msec = std::chrono::milliseconds;
		utils::Stopwatch sw;
		solver->solve({ msec(500), [](std::uint64_t, std::uint64_t) { return false; } });
		const auto elapsedSec = sw.elapsed();
		EXPECT_GE(elapsedSec.count(), 0.5);
		// Let's give it much time, as there may be a debug configuration, a busy machine, etc. 
		// The main thing is to make sure the task is canceled and not frozen.
		EXPECT_LT(elapsedSec.count(), 60.5);
	}


	INSTANTIATE_TEST_SUITE_P(
		ISolverRealPuzzleBruteForceSequenced,
		ISolverRealPuzzleTests,
		testing::Values(SolverConfig{ Method::BruteForce, BFConfig{.prefilter = true, .execution = ExecPolicy::Sequenced} }));

	INSTANTIATE_TEST_SUITE_P(
		ISolverRealPuzzleBruteForceParallel,
		ISolverRealPuzzleTests,
		testing::Values(SolverConfig{ Method::BruteForce, BFConfig{.prefilter = true, .execution = ExecPolicy::Parallel} }));

	INSTANTIATE_TEST_SUITE_P(
		ISolverRealPuzzleReasoning,
		ISolverRealPuzzleTests,
		testing::Values(SolverConfig{ Method::Reasoning, {} }));
}