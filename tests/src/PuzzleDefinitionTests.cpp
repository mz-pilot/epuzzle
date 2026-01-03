#include "epuzzle/PuzzleDefinition.h"
#include "epuzzle/Exceptions.h"

namespace epuzzle::tests
{

    TEST(PuzzleDefinitionTests, ValidatePuzzle0x0Throw)
    {
        ASSERT_THROW(validate(PuzzleDefinition{ {}, {}, {} }), PuzzleLogicError);
    }

    TEST(PuzzleDefinitionTests, ValidatePuzzleNx0Throw)
    {
        EXPECT_THROW(validate(PuzzleDefinition{ {"NameA"}, {}, {} }), PuzzleLogicError);
        EXPECT_THROW(validate(PuzzleDefinition{ {"NameA", "NameB"}, {}, {} }), PuzzleLogicError);
        EXPECT_THROW(validate(PuzzleDefinition{ {"NameA", "NameB", "NameC"}, {}, {} }), PuzzleLogicError);
    }

    TEST(PuzzleDefinitionTests, ValidatePuzzle1x1Throw)
    {
        ASSERT_THROW(validate(PuzzleDefinition{ {"NameA"}, {{{"attr1"}, {"attr1_val1"}}}, {} }), PuzzleLogicError);
    }
    TEST(PuzzleDefinitionTests, ValidatePuzzle1x2Throw)
    {
        ASSERT_THROW(validate(PuzzleDefinition{ {"NameA"}, {{{"attr1"}, {"attr1_val1"}}, {{"attr2"}, {"attr2_val1"}}}, {} }), PuzzleLogicError);
    }

    TEST(PuzzleDefinitionTests, ValidatePuzzle2x1Throw)
    {
        ASSERT_THROW(validate(PuzzleDefinition{ {"NameA", "NameB"}, {{{"attr1"}, {"attr1_val1", "attr1_val2"}}}, {} }), PuzzleLogicError);
    }

    TEST(PuzzleDefinitionTests, ValidatePuzzle2x2NoConstraintsThrow)
    {
        ASSERT_THROW(validate(PuzzleDefinition
            {
                {"NameA", "NameB"},
                {
                    {{"attr1"}, {"attr1_val1", "attr1_val2"}},
                    {{"attr2"}, {"attr1_val2", "attr2_val2"}}},
                {}
            }), PuzzleLogicError);
    }

}
