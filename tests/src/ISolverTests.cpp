#include "epuzzle/ISolver.h"

namespace epuzzle::tests
{
	namespace
	{
		using BFConfig = SolverConfig::BruteForce;
		using Method = SolverConfig::SolvingMethod;
		using ExecPolicy = SolverConfig::BruteForce::ExecPolicy;
		using Fact = PuzzleData::Fact;
		using Comparison = PuzzleData::Comparison;
		using Relation = PuzzleData::Comparison::Relation;
		using ConstraintDefs = std::vector<std::variant<PuzzleData::Fact, PuzzleData::Comparison>>;
	}

	class ISolverTests : public testing::TestWithParam<SolverConfig>
	{
	public:

		static constexpr bool NEGATE = true;
		
		void SetUp() override
		{
			if (GetParam().solvingMethod == SolverConfig::SolvingMethod::Reasoning)
				GTEST_SKIP() << "Reasoner implementation coming in next version!";
		}

		static constexpr PuzzleData puzzle_2x2()
		{
			return PuzzleData
			{
				{ "NameA", "NameB" },
				{
					{{"attr1"}, {"attr1_val1", "attr1_val2" }},
					{{"attr2"}, {"attr2_val1", "attr2_val2" }}
				},
				{}
			};
		}

		static constexpr PuzzleData puzzle_2x3()
		{
			return PuzzleData
			{
				{ "NameA", "NameB" },
				{
					{{"attr1"}, {"attr1_val1", "attr1_val2" }},
					{{"attr2"}, {"attr2_val1", "attr2_val2" }},
					{{"attr3"}, {"attr3_val1", "attr3_val2" }}
				},
				{}
			};
		}

		static constexpr PuzzleData puzzle_3x2()
		{
			return PuzzleData
			{
				{ "NameA", "NameB", "NameC" },
				{
					{{"attr1"}, {"attr1_val1", "attr1_val2", "attr1_val3" }},
					{{"attr2"}, {"attr2_val1", "attr2_val2", "attr2_val3" }}
				},
				{}
			};
		}

		static constexpr PuzzleData puzzle_3x3()
		{
			return PuzzleData
			{
				{ "NameA", "NameB", "NameC"},
				{
					{{"attr1"}, {"attr1_val1", "attr1_val2", "attr1_val3" }},
					{{"attr2"}, {"attr2_val1", "attr2_val2", "attr2_val3" }},
					{{"attr3"}, {"attr3_val1", "attr3_val2", "attr3_val3" }}
				},
				{}
			};
		}

		auto solve(PuzzleData&& data, ConstraintDefs&& constraints)
		{
			data.constraints = std::move(constraints);
			std::unique_ptr<ISolver> solver;
			std::vector<PuzzleSolution> solutions;
			EXPECT_NO_THROW(solver = ISolver::create(GetParam(), std::move(data)));
			if (solver)
			{
				EXPECT_NO_THROW(solutions = solver->solve({}));
			}
			return solutions;
		}

		bool noSolutionFor(PuzzleData&& data, ConstraintDefs&& constraints)
		{
			return solve(std::move(data), std::move(constraints)).empty();
		}

		bool hasSolutionSet(PuzzleData&& data, ConstraintDefs&& constraints, std::vector<PuzzleSolution>&& expectedSolutions)
		{
			auto solutions = solve(std::move(data), std::move(constraints));
			EXPECT_EQ(solutions.size(), expectedSolutions.size());

			auto compareSolutions = [](const PuzzleSolution& l, const PuzzleSolution& r)
				{
					if (l.attributes.size() != r.attributes.size())
						return false;

					for (size_t i = 0; i < l.attributes.size(); ++i)
					{
						const auto& lAttr = l.attributes[i];
						const auto& rAttr = r.attributes[i];
						if (lAttr.type != rAttr.type || lAttr.values != rAttr.values)
							return false;
					}
					return true;
				};

			for (const auto& expected : expectedSolutions)
			{
				if (1 != std::count_if(solutions.begin(), solutions.end(), [compareSolutions, &expected](const auto& solution)
					{
						return compareSolutions(solution, expected);
					}))
				{
					return false;
				}
			}
			return true;
		}

	};

	TEST_P(ISolverTests, puzzle2x2SolutionCount0)
	{
		EXPECT_TRUE(noSolutionFor(puzzle_2x2(), 
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, NEGATE }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val2"}} }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{ {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val1"}} }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{ {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val2"}} },
				Fact{ {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val1"}} }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val2"}}, NEGATE },
				Fact{ {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val1"}}}
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, NEGATE },
				Fact{ {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val1"}}, NEGATE }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::Before },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::After }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::Before },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::After },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::Before }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::ImmediateLeft },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::ImmediateRight }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::Before },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::ImmediateRight }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::ImmediateLeft },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::After }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Comparison{ {{"attr1"}, {"attr1_val1"}}, {{"attr1"}, {"attr1_val2"}}, {"attr2"}, Relation::Before },
				Comparison{ {{"attr1"}, {"attr1_val2"}}, {{"attr1"}, {"attr1_val1"}}, {"attr2"}, Relation::ImmediateLeft }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameB"}}, {"attr1"}, Relation::After }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, NEGATE },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameB"}}, {"attr1"}, Relation::Before }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{ {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val1"}} },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameB"}}, {"attr1"}, Relation::Before }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, NEGATE },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::ImmediateLeft },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, {"attr1"}, Relation::ImmediateRight }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Comparison{ {{"attr2"}, {"attr2_val1"}}, {{"person"}, {"NameB"}}, {"attr1"}, Relation::After },
				Fact{       {{"person"}, {"NameA"}}, {{"attr2"}, {"attr2_val1"}} },
				Fact{       {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val2"}} }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_2x2(),
			{
				Comparison{ {{"attr1"}, {"attr1_val2"}}, {{"person"}, {"NameA"}}, {"attr2"}, Relation::Before },
				Fact{       {{"person"}, {"NameA"}}, {{"attr2"}, {"attr2_val1"}} },
				Fact{       {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val2"}} }
			}));
	}

	TEST_P(ISolverTests, Puzzle2x2SolutionCount1)
	{
		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(), 
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{ {{"person"}, {"NameB"}}, {{"attr2"}, {"attr2_val2"}} }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{ {{"person"}, {"NameA"}}, {{"attr2"}, {"attr2_val2"}} }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val2", "attr2_val1"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val2"}}, NEGATE },
				Fact{ {{"person"}, {"NameB"}}, {{"attr2"}, {"attr2_val2"}} }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, NEGATE },
				Fact{ {{"person"}, {"NameB"}}, {{"attr2"}, {"attr2_val2"}}, NEGATE }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val2", "attr1_val1"}},
					{"attr2", {"attr2_val2", "attr2_val1"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Fact{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{ {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val2"}} },
				Fact{ {{"person"}, {"NameB"}}, {{"attr2"}, {"attr2_val2"}} }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Comparison{ {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val2"}}, {"attr1"}, Relation::Before },
				Comparison{ {{"person"}, {"NameB"}}, {{"attr2"}, {"attr2_val1"}}, {"attr2"}, Relation::ImmediateRight }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameB"}}, {"attr1"}, Relation::ImmediateLeft },
				Comparison{ {{"attr1"}, {"attr1_val1"}}, {{"attr2"}, {"attr2_val2"}}, {"attr2"}, Relation::Adjacent },
				Comparison{ {{"person"}, {"NameB"}}, {{"person"}, {"NameA"}}, {"attr2"}, Relation::After }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val2"}}, NEGATE },
				Comparison{ {{"person"}, {"NameA"}}, {{"attr2"}, {"attr2_val1"}}, {"attr2"}, Relation::Adjacent }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val2", "attr2_val1"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Comparison{ {{"attr1"}, {"attr1_val1"}}, {{"attr1"}, {"attr1_val2"}}, {"attr2"}, Relation::ImmediateLeft },
				Comparison{ {{"attr2"}, {"attr2_val1"}}, {{"attr2"}, {"attr2_val2"}}, {"attr1"}, Relation::Before },
				Fact{       {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val1"}}, NEGATE }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{       {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val2"}} },
				Comparison{ {{"attr1"}, {"attr1_val1"}}, {{"attr1"}, {"attr1_val2"}}, {"attr2"}, Relation::Adjacent },
				Comparison{ {{"attr1"}, {"attr1_val1"}}, {{"attr2"}, {"attr2_val2"}}, {"attr2"}, Relation::Before }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}}
			}));
	}

	TEST_P(ISolverTests, Puzzle2x2SolutionCount2)
	{
		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}}, NEGATE },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameB"}}, {"attr2"}, Relation::Adjacent }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val2", "attr1_val1"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}},
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val2", "attr1_val1"}},
					{"attr2", {"attr2_val2", "attr2_val1"}}
				}}
			}));

		EXPECT_TRUE(hasSolutionSet(puzzle_2x2(),
			{
				Comparison{ {{"attr1"}, {"attr1_val1"}}, {{"attr1"}, {"attr1_val2"}}, {"attr2"}, Relation::Before }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}}
				}},
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val2", "attr1_val1"}},
					{"attr2", {"attr2_val2", "attr2_val1"}}
				}}
			}));
	}

	TEST_P(ISolverTests, Puzzle2x3SolutionCount1)
	{
		EXPECT_TRUE(hasSolutionSet(puzzle_2x3(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val2"}}, NEGATE },
				Fact{       {{"person"}, {"NameB"}}, {{"attr2"}, {"attr2_val1"}}, NEGATE },
				Comparison{ {{"attr1"}, {"attr1_val2"}}, {{"attr2"}, {"attr2_val1"}}, {"attr3"}, Relation::After }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB"}},
					{"attr1", {"attr1_val1", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2"}},
					{"attr3", {"attr3_val1", "attr3_val2"}}
				}}
			}));
	}

	TEST_P(ISolverTests, Puzzle3x2SolutionCount2)
	{
		EXPECT_TRUE(hasSolutionSet(puzzle_3x2(),
			{
				Fact{       {{"person"}, {"NameB"}}, {{"attr1"}, {"attr1_val2"}} },
				Fact{       {{"person"}, {"NameB"}}, {{"attr2"}, {"attr2_val2"}} },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameC"}}, {"attr1"}, Relation::Before }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB", "NameC"}},
					{"attr1", {"attr1_val1", "attr1_val2", "attr1_val3"}},
					{"attr2", {"attr2_val1", "attr2_val2", "attr2_val3"}}
				}},
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB", "NameC"}},
					{"attr1", {"attr1_val1", "attr1_val2", "attr1_val3"}},
					{"attr2", {"attr2_val3", "attr2_val2", "attr2_val1"}}
				}}
			}));
	}

	TEST_P(ISolverTests, Puzzle3x2SolutionCount4)
	{
		EXPECT_TRUE(hasSolutionSet(puzzle_3x2(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{       {{"person"}, {"NameA"}}, {{"attr2"}, {"attr2_val1"}} }
			},
			{
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB", "NameC"}},
					{"attr1", {"attr1_val1", "attr1_val2", "attr1_val3"}},
					{"attr2", {"attr2_val1", "attr2_val2", "attr2_val3"}}
				}},
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB", "NameC"}},
					{"attr1", {"attr1_val1", "attr1_val2", "attr1_val3"}},
					{"attr2", {"attr2_val1", "attr2_val3", "attr2_val2"}}
				}},
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB", "NameC"}},
					{"attr1", {"attr1_val1", "attr1_val3", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val2", "attr2_val3"}}
				}},
				PuzzleSolution
				{{
					{"person", {"NameA", "NameB", "NameC"}},
					{"attr1", {"attr1_val1", "attr1_val3", "attr1_val2"}},
					{"attr2", {"attr2_val1", "attr2_val3", "attr2_val2"}}
				}}
			}));
	}

	TEST_P(ISolverTests, Puzzle3x3SolutionCount0)
	{
		EXPECT_TRUE(noSolutionFor(puzzle_3x3(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{       {{"person"}, {"NameC"}}, {{"attr1"}, {"attr1_val3"}} },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameC"}}, {"attr1"}, Relation::Adjacent }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_3x3(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{       {{"person"}, {"NameC"}}, {{"attr1"}, {"attr1_val3"}} },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameC"}}, {"attr1"}, Relation::ImmediateLeft }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_3x3(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{       {{"person"}, {"NameC"}}, {{"attr1"}, {"attr1_val3"}} },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameC"}}, {"attr1"}, Relation::After }
			}));

		EXPECT_TRUE(noSolutionFor(puzzle_3x3(),
			{
				Fact{       {{"person"}, {"NameA"}}, {{"attr1"}, {"attr1_val1"}} },
				Fact{       {{"person"}, {"NameC"}}, {{"attr1"}, {"attr1_val3"}} },
				Comparison{ {{"person"}, {"NameA"}}, {{"person"}, {"NameC"}}, {"attr1"}, Relation::ImmediateRight }
			}));
	}


	INSTANTIATE_TEST_SUITE_P(
		ISolverBruteForce,
		ISolverTests,
		testing::Values(SolverConfig{ Method::BruteForce, BFConfig{.prefilter = false, .execution = ExecPolicy::Sequenced} }));

	INSTANTIATE_TEST_SUITE_P(
		ISolverBruteForcePrefilter,
		ISolverTests,
		testing::Values(SolverConfig{ Method::BruteForce, BFConfig{.prefilter = true, .execution = ExecPolicy::Sequenced} }));

	INSTANTIATE_TEST_SUITE_P(
		ISolverReasoning,
		ISolverTests,
		testing::Values(SolverConfig{ Method::Reasoning, {} }));
}
