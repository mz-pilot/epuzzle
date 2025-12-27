#pragma once
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>

#include "SolverConfig.h"
#include "PuzzleDefinition.h"
#include "PuzzleSolution.h"

namespace epuzzle
{

    // Solver. The main interface for client code.
    class Solver
    {
    public:
        static std::unique_ptr<Solver> create(SolverConfig, PuzzleDefinition);
        virtual ~Solver() = default;

        // Parameter object. The callback executes in solve()'s calling thread. Return false to cancel the operation.
        struct SolveOptions
        {
            std::chrono::milliseconds progressInterval = std::chrono::milliseconds(1000);
            std::function<bool(std::uint64_t total, std::uint64_t current)> progressCallback = [](auto, auto) { return true; };
        };
        virtual std::vector<PuzzleSolution> solve(SolveOptions) = 0;
    };

}
