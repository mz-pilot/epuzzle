#include <tomlplusplus/toml.hpp>

#include "epuzzle/Exceptions.h"
#include "epuzzle/PuzzleParser.h"

#define ENSURE_PARSE(expression, message) \
        ENSURE_SPEC(expression, PuzzleDataParseError, message)

namespace epuzzle
{

namespace
{
    void checkFormatVersion(const toml::table& root)
    {
        constexpr auto valueName = "epuzzle_format_version";
        constexpr auto minSupportedVersion = 1u;
        constexpr auto maxSupportedVersion = 1u;
        const auto version = root[valueName].value_or(0u);
        ENSURE_PARSE(minSupportedVersion <= version && version <= maxSupportedVersion, "Unsupported '" << valueName <<
            "': " << version << "! minSupportedVersion = " << minSupportedVersion << ", maxSupportedVersion = " << maxSupportedVersion);
    }

    PuzzleDefinition parseTomlRoot(const toml::table& root)
    {
        checkFormatVersion(root);

        PuzzleDefinition data;
        using namespace std::string_view_literals;
        root.for_each([&data](const toml::key& key, const toml::array& arr)
            {
                auto readVec = [](const toml::array& arr)
                    {
                        std::vector<std::string> vec;
                        vec.reserve(arr.size());
                        arr.for_each([&vec](const toml::value<std::string>& value) { vec.push_back(value.get()); });
                        return vec;
                    };

                if (key == "person")
                {
                    data.persons = readVec(arr);
                }
                else if (key == "constraints")
                {
                    arr.for_each([&data](const toml::table& constraintTable)
                        {
                            auto readAttr = [&constraintTable](std::string_view name) -> PuzzleDefinition::Attribute
                                {
                                    PuzzleDefinition::Attribute attr;
                                    const auto inlineTable = constraintTable[name].as_table();
                                    if (inlineTable && inlineTable->size() == 1)
                                    {
                                        attr.type = inlineTable->cbegin()->first;
                                        attr.value = inlineTable->cbegin()->second.value_or(std::string());
                                    }
                                    return attr;
                                };

                            const auto type = constraintTable["type"].value_or(""sv);
                            if (type == "fact")
                            {
                                PuzzleDefinition::Fact fact{ readAttr("first"), readAttr("second") };
                                if (fact.second.value.length() > 0 && fact.second.value[0] == '!')
                                {
                                    fact.secondNegate = true;
                                    fact.second.value.erase(0, 1);
                                }
                                data.constraints.push_back(std::move(fact));
                            }
                            else if (type == "comparison")
                            {
                                auto compareBy = constraintTable["compare_by"].value_or(""sv);
                                ENSURE_PARSE(!compareBy.empty(),
                                    "'compare_by' parameter is abscent for constraint type 'comparison'! table source: " << constraintTable.source());

                                const auto relStr = constraintTable["relation"].value_or(""sv);
                                using Relation = PuzzleDefinition::Comparison::Relation;
                                Relation rel;
                                if (relStr == "immediate_left")    rel = Relation::ImmediateLeft;
                                else if (relStr == "immediate_right")   rel = Relation::ImmediateRight;
                                else if (relStr == "adjacent")          rel = Relation::Adjacent;
                                else if (relStr == "before")            rel = Relation::Before;
                                else if (relStr == "after")             rel = Relation::After;
                                else ENSURE_PARSE(false, "Unsupported relation type: " << relStr << ", table source: " << constraintTable.source());

                                data.constraints.emplace_back(std::in_place_type<PuzzleDefinition::Comparison>,
                                    readAttr("first"), readAttr("second"), std::string(compareBy), rel);
                            }
                            else
                                ENSURE_PARSE(false, "Unsupported constraint type: " << type << ", table source: " << constraintTable.source());
                        });
                }
                else // other attributes
                {
                    data.attributes.emplace_back(std::string(key), readVec(arr));
                }
            });

        return data;
    }

} // namespace

    PuzzleDefinition parseText(std::string_view text)
    {
        try
        {
            return parseTomlRoot(toml::parse(text));
        }
        catch (const toml::parse_error& pe)
        {
            ENSURE_PARSE(false, "toml++ exception! " << pe);
        }
        catch (const std::exception& e)
        {
            throw PuzzleDataParseError(e.what());
        }
    }

    PuzzleDefinition parseFile(std::string_view filePath)
    {
        try
        {
            return parseTomlRoot(toml::parse_file(filePath));
        }
        catch (const toml::parse_error& pe)
        {
            ENSURE_PARSE(false, "toml++ exception! " << pe);
        }
        catch (const std::exception& e)
        {
            throw PuzzleDataParseError(std::format("{}; filePath = {}", e.what(), filePath));
        }

    }

} // namespace epuzzle
