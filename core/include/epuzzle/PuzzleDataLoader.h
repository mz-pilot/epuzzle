#pragma once
#include <string_view>

#include "PuzzleData.h"

namespace epuzzle
{
    PuzzleData puzzleDataFromText(std::string_view text); // throw PuzzleDataParseError

    PuzzleData puzzleDataFromFile(std::string_view filePath); // throw PuzzleDataParseError
}
