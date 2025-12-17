#include <format>
#include <ranges>
#include <string_view>

#include <cxxopts/cxxopts.hpp>
#include <magic_enum/magic_enum.hpp>

#include "ProgramOptions.h"

namespace cli
{

	struct EnumHelper
	{
		template <typename TEnum>
		static std::string names()
		{
			static const auto prefetched = []()
				{
					std::string str;
					constexpr auto namesArr = magic_enum::enum_names<TEnum>();
					for (auto i : std::views::iota(0u, namesArr.size()))
					{
						str += namesArr[i];
						if (i < namesArr.size() - 1)
							str += '|';
					}
					return str;
				}();
			return prefetched;
		}

		template <typename TEnum>
		static constexpr std::string_view name(TEnum val)
		{
			return magic_enum::enum_name(val);
		}

		template <typename TEnum>
		static constexpr TEnum cast(std::string_view sv)
		{
			if (const auto optVal = magic_enum::enum_cast<TEnum>(sv))
				return *optVal;
			else
				throw std::runtime_error(std::format("Unexpected option value: {}, expected one of: {}", sv, names<TEnum>()));
		}
	};

	std::optional<ProgramOptions> getProgramOptions(int argc, char* argv[])
	{
		using Method = epuzzle::SolverConfig::SolvingMethod;
		using ExecPolicy = epuzzle::SolverConfig::BruteForce::ExecPolicy;
		const auto appName = "epuzzle";
		cxxopts::Options optsManager(appName, "Solver of puzzles such as Einstein's puzzle (Zebra puzzle) and similar ones");
		optsManager.set_width(110);
		optsManager.add_options()
			("m,method", "[required] Solving method, where arg is: " + EnumHelper::names<Method>(), cxxopts::value<std::string>())
			("p,prefilter", "[optional] Enable prefilter for bruteforce", cxxopts::value<bool>()->default_value("true"))
			("e,execpolicy", "[required if BruteForce] Execution policy, where arg is: " + EnumHelper::names<ExecPolicy>(),
				cxxopts::value<std::string>())
			("f,file", "[required] Path to puzzle data file", cxxopts::value<std::string>())
			("h,help", "Print usage");

		const auto parsedOpts = optsManager.parse(argc, argv);
		if (parsedOpts.count("help") || parsedOpts.arguments().empty())
		{
			std::cout << "\n" << optsManager.help() << "\n";
			std::cout << "Examples: \n\n";

			std::cout << "# Parallel brute force\n";
			std::cout << appName << " --method " << EnumHelper::name(Method::BruteForce)
				<< " --execpolicy " << EnumHelper::name(ExecPolicy::Parallel)
				<< " --file puzzle_examples/einsteins.toml\n\n";

			std::cout << "# Disable prefilter for total sequenced brute force\n";
			std::cout << appName << " -m " << EnumHelper::name(Method::BruteForce)
				<< " --prefilter=false --execpolicy " << EnumHelper::name(ExecPolicy::Sequenced) << " -f test.toml\n\n";

			std::cout << "# Basic usage with reasoning\n";
			std::cout << appName << " -m " << EnumHelper::name(Method::Reasoning) << " -f \"Zebra.toml\"\n\n";
			std::cout << std::endl;
			return {};
		}

		ProgramOptions programOpts;
		programOpts.config.solvingMethod = EnumHelper::cast<Method>(parsedOpts["method"].as<std::string>());
		if (programOpts.config.solvingMethod == Method::BruteForce)
		{
			programOpts.config.bruteForce =
			{
				.prefilter = parsedOpts["prefilter"].as<bool>(),
				.execution = EnumHelper::cast<ExecPolicy>(parsedOpts["execpolicy"].as<std::string>())
			};
		}
		programOpts.puzzleFilePath = parsedOpts["file"].as<std::string>();
		return programOpts;
	}

}