#include "epuzzle/Solver.h"
#include "epuzzle/PuzzleParser.h"
#include "TestUtils.h"

namespace epuzzle::tests
{

    class SolverRealPuzzleTests : public testing::TestWithParam<SolverConfig>
    {
    public:
    protected:
        void SetUp() override
        {
            if (GetParam().solvingMethod == SolverConfig::SolvingMethod::Deductive)
                GTEST_SKIP() << "Deductive implementation coming in next version!";
        }

        static auto solve(std::string_view fileName)
        {
            std::unique_ptr<Solver> solver;
            std::vector<PuzzleSolution> solutions;
            EXPECT_NO_THROW(solver = Solver::create(GetParam(), parseFile(test_utils::puzzlePath(fileName))));
            if (solver)
            {
                EXPECT_NO_THROW(solutions = solver->solve({}));
            }
            return solutions;
        }
    };

    namespace
    {
        using BFConfig = SolverConfig::BruteForceConfig;
        using Method = SolverConfig::SolvingMethod;
        using ExecPolicy = SolverConfig::BruteForceConfig::ExecPolicy;
    }

    TEST_P(SolverRealPuzzleTests, FourGirlsSolutionCount128)
    {
        const auto solutions = solve("four_girls.toml");
        ASSERT_EQ(solutions.size(), 128);
    }

    TEST_P(SolverRealPuzzleTests, EinsteinsSolutionCount1Correct)
    {
        const auto solutions = solve("einsteins.toml");
        ASSERT_EQ(solutions.size(), 1);
        EXPECT_TRUE(personHasAttribute(solutions.front(), "Норвежец", "beverage", "вода"));
        EXPECT_TRUE(personHasAttribute(solutions.front(), "Японец", "pet", "зебра"));
    }

    TEST_P(SolverRealPuzzleTests, SolveCanceled)
    {
        std::unique_ptr<Solver> solver;
        // Use hard puzzle with long compute (weeks)
        ASSERT_NO_THROW(solver = Solver::create(GetParam(), parseFile(test_utils::puzzlePath("six_houses.toml"))));
        ASSERT_TRUE(static_cast<bool>(solver));
        using msec = std::chrono::milliseconds;
        const utils::Stopwatch sw;
        solver->solve({ .progressInterval = msec(500), .progressCallback = [](std::uint64_t, std::uint64_t) { return false; } });
        const auto elapsedSec = sw.elapsed();
        EXPECT_GE(elapsedSec.count(), 0.5);
        // Let's give it much time, as there may be a debug configuration, a busy machine, etc. 
        // The main thing is to make sure the task is canceled and not frozen.
        EXPECT_LT(elapsedSec.count(), 60.5);
    }


    INSTANTIATE_TEST_SUITE_P(
        SolverRealPuzzleBruteForceSequential,
        SolverRealPuzzleTests,
        testing::Values(SolverConfig{ Method::BruteForce, BFConfig{.prefilter = true, .execution = ExecPolicy::Sequential} }));

    INSTANTIATE_TEST_SUITE_P(
        SolverRealPuzzleBruteForceParallel,
        SolverRealPuzzleTests,
        testing::Values(SolverConfig{ Method::BruteForce, BFConfig{.prefilter = true, .execution = ExecPolicy::Parallel} }));

    INSTANTIATE_TEST_SUITE_P(
        SolverRealPuzzleDeductive,
        SolverRealPuzzleTests,
        testing::Values(SolverConfig{ Method::Deductive, {} }));
}
