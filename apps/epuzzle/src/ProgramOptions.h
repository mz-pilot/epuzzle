#pragma once
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "epuzzle/SolverConfig.h"

namespace cli
{
	struct ProgramOptions
	{
		epuzzle::SolverConfig config;
		std::string puzzleFilePath;
	};

	std::optional<ProgramOptions> getProgramOptions(int argc, char* argv[]); // throw std::runtime_error
}