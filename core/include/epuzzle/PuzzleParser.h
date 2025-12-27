#pragma once
#include <string_view>

#include "PuzzleDefinition.h"

namespace epuzzle
{
    PuzzleDefinition parseText(std::string_view text); // throw PuzzleSyntaxError

    PuzzleDefinition parseFile(std::string_view filePath); // throw PuzzleSyntaxError
}
