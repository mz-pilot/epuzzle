#include "epuzzle/PuzzleParser.h"

namespace epuzzle::tests
{

    TEST(PuzzleParserTests, ParseTextOk)
    {
        static constexpr std::string_view text = R"(
            #
            # Original natural language description: four_girls.txt
            #
            # TOML fromat specification: https://toml.io/en/
            # TOML Validator: https://www.toml-lint.com/

            epuzzle_format_version = 1

            # person - mandatory attribute:
            person	= ["Anya", "Katya", "Lena", "Masha"]

            # next attributes are optional:
            age		= ["15", "16", "17", "18"]
            color	= ["pink", "blue", "green", "violet"]
            hobby	= ["art", "sport", "music", "books"]

            # сonstraints: 
            #   fact: "person" can only be the first attribute in a "fact"constraint;  second attribute value can start with '!' that means "NOT".
            #   comparison: possible relations: "immediate_left", "immediate_right", "adjacent", "before", "after".

            # Аня не любит розовый
            [[constraints]]
            type = "fact"
            first = { person = "Anya" }
            second = { color = "!pink" }

            # Аня не занимается спортом
            [[constraints]]
            type = "fact"
            first = { person = "Anya" }
            second = { hobby = "!sport" }

            # Та, кто любит голубой, младше Кати
            [[constraints]]
            type = "comparison"
            first = { color = "blue" }
            compare_by = "age"
            relation = "before"
            second = { person = "Katya" }

            # Маша старше любительницы музыки
            [[constraints]]
            type = "comparison"
            first = { person = "Masha" }
            compare_by = "age"
            relation = "after"
            second = { hobby = "music" }

            # Лена занимается рисованием
            [[constraints]]
            type = "fact"
            first = { person = "Lena" }
            second = { hobby = "art" }

            # Лена не любит фиолетовый
            [[constraints]]
            type = "fact"
            first = { person = "Lena" }
            second = { color = "!violet" }

            # Любительница зеленого цвета не читает книги
            [[constraints]]
            type = "fact"
            first = { color = "green" }
            second = { hobby = "!books" }
        )";

        using Fact = PuzzleDefinition::Fact;
        using Comparison = PuzzleDefinition::Comparison;
        using Relation = PuzzleDefinition::Comparison::Relation;

        PuzzleDefinition parsedData;
        ASSERT_NO_THROW(parsedData = parseText(text));

        constexpr bool negate = true;
        const PuzzleDefinition etalonData =
        {
            { "Anya", "Katya", "Lena", "Masha" },
            {
                {{"age"}, {"15", "16", "17", "18"}},
                {{"color"}, {"pink", "blue", "green", "violet"}},
                {{"hobby"}, {"art", "sport", "music", "books"}}
            },
            {
                Fact{ {{"person"}, {"Anya"}}, {{"color"}, {"pink"}}, negate },
                Fact{ {{"person"}, {"Anya"}}, {{"hobby"}, {"sport"}}, negate },
                Comparison{ {{"color"}, {"blue"}}, {{"person"}, {"Katya"}}, {"age"}, Relation::Before },
                Comparison{ {{"person"}, {"Masha"}}, {{"hobby"}, {"music"}}, {"age"}, Relation::After },
                Fact{ {{"person"}, {"Lena"}} , {{"hobby"}, {"art"}} },
                Fact{ {{"person"}, {"Lena"}} , {{"color"}, {"violet"}}, negate },
                Fact{ {{"color"}, {"green"}} , {{"hobby"}, {"books"}}, negate }
            }
        };

        // Для быстрой локализации ошибок проверим всё вручную с распечатыванием индексов проблемных элементов
        ASSERT_EQ(parsedData.persons, etalonData.persons);
        ASSERT_EQ(parsedData.attributes.size(), etalonData.attributes.size());
        for (size_t i = 0; i < parsedData.attributes.size(); ++i)
        {
            SCOPED_TRACE(testing::Message() << "At index " << i);
            EXPECT_EQ(parsedData.attributes[i].type, etalonData.attributes[i].type);
            EXPECT_EQ(parsedData.attributes[i].values, etalonData.attributes[i].values);
        }
        ASSERT_EQ(parsedData.constraints.size(), etalonData.constraints.size());
        for (size_t i = 0; i < parsedData.constraints.size(); ++i)
        {
            SCOPED_TRACE(testing::Message() << "At index " << i);
            const auto& parsedElem = parsedData.constraints[i];
            const auto& etalonElem = etalonData.constraints[i];
            EXPECT_EQ(parsedElem.index(), etalonElem.index());
            if (parsedElem.index() == etalonElem.index())
            {
                std::visit(utils::overloaded
                    {
                        [etalonElem](const Fact& fact)
                        {
                            const auto& etalonFact = std::get<Fact>(etalonElem);
                            EXPECT_EQ(fact.first.type, etalonFact.first.type);
                            EXPECT_EQ(fact.first.value, etalonFact.first.value);
                            EXPECT_EQ(fact.second.type, etalonFact.second.type);
                            EXPECT_EQ(fact.second.value, etalonFact.second.value);
                            EXPECT_EQ(fact.secondNegate, etalonFact.secondNegate);
                        },
                        [etalonElem](const Comparison& comparison)
                        {
                            const auto& etalonComparison = std::get<Comparison>(etalonElem);
                            EXPECT_EQ(comparison.first.type, etalonComparison.first.type);
                            EXPECT_EQ(comparison.first.value, etalonComparison.first.value);
                            EXPECT_EQ(comparison.second.type, etalonComparison.second.type);
                            EXPECT_EQ(comparison.second.value, etalonComparison.second.value);
                            EXPECT_EQ(comparison.compareBy, etalonComparison.compareBy);
                            EXPECT_EQ(comparison.relation, etalonComparison.relation);
                        },
                    }, parsedElem);
            }
        }
    }

}
