#include <format>
#include <ranges>
#include <string_view>

#include <cxxopts/cxxopts.hpp>
#include <magic_enum/magic_enum.hpp>

#include "ProgramOptions.h"

namespace cli
{
namespace 
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

	enum class PrefilterMode
	{
		Enabled,
		Disabled
	};

	constexpr auto appName = "epuzzle"; // let's not fetch it from argv[0] for now
}

	std::optional<ProgramOptions> getProgramOptions(int argc, char* argv[])
	{
		using Method = epuzzle::SolverConfig::SolvingMethod;
		using ExecPolicy = epuzzle::SolverConfig::BruteForce::ExecPolicy;
		// Note: cxxopts doesnt like string_view
		const std::string defaultMethod{ EnumHelper::name(Method::BruteForce) };
		const std::string defaultBfPrefilter{ EnumHelper::name(PrefilterMode::Enabled) };
		const std::string defaultBfExec{ EnumHelper::name(ExecPolicy::Parallel) };

		cxxopts::Options optsManager(appName, "Logic puzzle solver\nSolves Einstein's/Zebra Puzzle and similar constraint satisfaction puzzles.");
		optsManager.custom_help("--file <FILE> [OPTIONS]");
		optsManager.add_options()
			("f,file", "[required] Path to puzzle data file", cxxopts::value<std::string>())
			("m,method", "[optional] Solving method, where arg is: " + EnumHelper::names<Method>(), 
				cxxopts::value<std::string>()->default_value(defaultMethod))
			("p,prefilter", "[BruteForce only] Prefilter mode, where arg is: " + EnumHelper::names<PrefilterMode>(), 
				cxxopts::value<std::string>()->default_value(defaultBfPrefilter))
			("e,execpolicy", "[BruteForce only] Execution policy, where arg is: " + EnumHelper::names<ExecPolicy>(),
				cxxopts::value<std::string>()->default_value(defaultBfExec))
			("v,version", "Print version")
			("h,help", "Print usage");

		const auto parsedOpts = optsManager.parse(argc, argv);
		if (parsedOpts.count("help") || parsedOpts.arguments().size() <= 1)
		{
			std::cout << optsManager.help() << "\n";
			std::cout << "Examples: \n";

			std::cout << "# Quickly test the solver (default mode)\n";
			std::cout << appName << " --file puzzle_examples/einsteins.toml\n\n";

			std::cout << "# Basic usage with reasoning\n";
			std::cout << appName << " -f \"My puzzle.toml\"" << " -m " << EnumHelper::name(Method::Reasoning) << "\n\n";

			std::cout << "# Parallel bruteforce\n";
			std::cout << appName << " --file Zebra.toml" 
				<< " --method " << EnumHelper::name(Method::BruteForce)
				<< " --execpolicy " << EnumHelper::name(ExecPolicy::Parallel) << "\n\n";

			std::cout << "# Disable prefilter for Sequenced bruteforce\n";
			std::cout << appName << " -f test.toml"
				<< " -m " << EnumHelper::name(Method::BruteForce)
				<< " -p " << EnumHelper::name(PrefilterMode::Disabled) 
				<< " -e " << EnumHelper::name(ExecPolicy::Sequenced);
			
			std::cout << std::endl;
			return {};
		}
		else if (parsedOpts.contains("version") && parsedOpts.arguments().size() == 1)
		{
			printVersion();
			return {};
		}

		ProgramOptions programOpts;
		programOpts.config.solvingMethod = EnumHelper::cast<Method>(parsedOpts["method"].as<std::string>());
		if (programOpts.config.solvingMethod == Method::BruteForce)
		{
			programOpts.config.bruteForce =
			{
				.prefilter = (PrefilterMode::Enabled == EnumHelper::cast<PrefilterMode>(parsedOpts["prefilter"].as<std::string>())),
				.execution = EnumHelper::cast<ExecPolicy>(parsedOpts["execpolicy"].as<std::string>())
			};
		}
		programOpts.puzzleFilePath = parsedOpts["file"].as<std::string>();
		return programOpts;
	}

	void printVersion()
	{
		constexpr auto epuzzle_version = "0.0.0";
		std::cout << appName << " version " << epuzzle_version << std::endl;
	}
}