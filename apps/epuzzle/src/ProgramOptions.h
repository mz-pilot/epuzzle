#pragma once
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "epuzzle/SolverConfig.h"
#include "version.h"

namespace cli
{

    struct ProgramOptions
    {
        epuzzle::SolverConfig config;
        std::string puzzleFilePath;
    };

    std::optional<ProgramOptions> getProgramOptions(int argc, char* argv[]); // throw std::runtime_error  // NOLINT(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)

    void printVersion();
}
