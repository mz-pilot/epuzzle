#include "epuzzle/PuzzleData.h"
#include "epuzzle/Exceptions.h"

namespace epuzzle::tests
{

    TEST(PuzzleDataTests, ValidatePuzzle0x0Throw)
    {
        ASSERT_THROW(validate(PuzzleData{ {}, {}, {} }), PuzzleDataLogicError);
    }

    TEST(PuzzleDataTests, ValidatePuzzleNx0Throw)
    {
        EXPECT_THROW(validate(PuzzleData{ {"NameA"}, {}, {} }), PuzzleDataLogicError);
        EXPECT_THROW(validate(PuzzleData{ {"NameA", "NameB"}, {}, {} }), PuzzleDataLogicError);
        EXPECT_THROW(validate(PuzzleData{ {"NameA", "NameB", "NameC"}, {}, {} }), PuzzleDataLogicError);
    }

    TEST(PuzzleDataTests, ValidatePuzzle1x1Throw)
    {
        ASSERT_THROW(validate(PuzzleData{ {"NameA"}, {{{"attr1"}, {"attr1_val1"}}}, {} }), PuzzleDataLogicError);
    }
    TEST(PuzzleDataTests, ValidatePuzzle1x2Throw)
    {
        ASSERT_THROW(validate(PuzzleData{ {"NameA"}, {{{"attr1"}, {"attr1_val1"}}, {{"attr2"}, {"attr2_val1"}}}, {} }), PuzzleDataLogicError);
    }

    TEST(PuzzleDataTests, ValidatePuzzle2x1Throw)
    {
        ASSERT_THROW(validate(PuzzleData{ {"NameA", "NameB"}, {{{"attr1"}, {"attr1_val1", "attr1_val2"}}}, {} }), PuzzleDataLogicError);
    }

    TEST(PuzzleDataTests, ValidatePuzzle2x2NoConstraintsThrow)
    {
        ASSERT_THROW(validate(PuzzleData
            {
                {"NameA", "NameB"},
                {
                    {{"attr1"}, {"attr1_val1", "attr1_val2"}},
                    {{"attr2"}, {"attr1_val2", "attr2_val2"}}},
                {}
            }), PuzzleDataLogicError);
    }

}
