#pragma once
#include <stdexcept>

namespace epuzzle
{
    // Exceptions, that may be thrown into the client code and may be fixed by user. Other (internal) errors have standard types like std::logic_error.

    class SolverConfigError final : public std::runtime_error { using std::runtime_error::runtime_error; };

    class PuzzleDataParseError final : public std::runtime_error { using std::runtime_error::runtime_error; };

    class PuzzleDataLogicError final : public std::logic_error { using std::logic_error::logic_error; };

    class IncompatibleMethodError final : public std::runtime_error { using std::runtime_error::runtime_error; };

}
