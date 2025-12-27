#include <ranges>

#include "utils/Diagnostics.h"
#include "utils/Math.h"
#include "utils/platform/ConsoleUtils.h"

#include "epuzzle/PuzzleParser.h"
#include "epuzzle/Exceptions.h"
#include "epuzzle/Solver.h"

#include "ProgramOptions.h"

int main(int argc, char* argv[])
{
    int retcode = EXIT_SUCCESS;
    utils::Stopwatch sw;
    try
    {
        platform::setupConsole();

        auto options = cli::getProgramOptions(argc, argv);
        if (!options)
            return EXIT_SUCCESS;

        cli::printVersion();
        std::cout << "Config: " << options->config << "\n";
        auto puzzleDefinition = epuzzle::parseFile(options->puzzleFilePath);
        std::cout << "Puzzle loaded: \n" << puzzleDefinition << "\n";
        std::cout << "Preparing ... \n";
        auto solver = epuzzle::Solver::create(std::move(options->config), std::move(puzzleDefinition));
        std::cout << "Preparing done.\n";

        using namespace utils;
        using namespace std::chrono_literals;
        auto solutions = solver->solve({ 900ms, [](std::uint64_t total, std::uint64_t current)
            {
                const double percents = static_cast<double>(current) / static_cast<double>(std::max(1_u64, total)) * 100.0;
                std::cout << "\rSolving ...  " << std::fixed << std::setprecision(3) << percents << " %   " << std::flush;
                return true; // you can return false in GUI version if user pressed "cancel" button
            } });

        std::cout << "\nSolving finished!\n";
        std::cout << "Solutions found: " << solutions.size() << "\n";
        for (auto solNum : std::views::iota(0ul, solutions.size()))
        {
            std::cout << "Solution #" << solNum + 1 << ":\n";
            std::cout << solutions[solNum] << "\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nException occured! " << e.what() << "\n";
        retcode = EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "\nUnknown exception occured!\n" << std::endl;
        retcode = EXIT_FAILURE;
    }
    std::cout << "Elapsed: " << sw.elapsed() << std::endl;
    return retcode;
}
