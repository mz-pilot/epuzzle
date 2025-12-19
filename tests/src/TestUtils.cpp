#include "TestUtils.h"

namespace test_utils
{
    std::string puzzlePath(std::string_view filename)
    {
        return std::format("{}/{}", EPUZZLE_TEST_SAMPLES_DIR, filename);
    }
}
