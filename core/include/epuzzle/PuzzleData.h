#pragma once
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace epuzzle
{

	// Data Transfer Object (DTO) - input configuration: attributes descriptions, possible values and configurations constraints.
	struct PuzzleData
	{
		// Example: type = "color", values = "pink", "blue", "green", "violet" (all possible)
		struct AttributeDescription
		{
			std::string type;
			std::vector<std::string> values;
		};

		// Example: "house_color" - "green"
		struct Attribute
		{
			std::string type;
			std::string value;
		};

		// Fact of attribute bundle. 
		// Example: first(type = "person", value = "Bob"), second(type = "color", value = "green"), secondNegate(true) 
		//  - means "Bob likes not-green"
		struct Fact
		{
			Attribute first;
			Attribute second;
			bool secondNegate = false;
		};

		// Compare two persons by third attribute.
		// Example: first(type = "house_color", value = "pink"), compareBy("house_position") relation(ImmediateRight) second(type = "person", value = "Bob")
		//  - means "Pink house position is immediate right than Bob's house
		struct Comparison
		{
			enum class Relation
			{
				Uninitialized,
				ImmediateLeft,
				ImmediateRight,
				Adjacent,
				Before,
				After
			};

			Attribute first;
			Attribute second;
			std::string compareBy;
			Relation relation = Relation::Uninitialized;
		};

		// A person is a special attribute.It identifies the owner of any attribute. It's type name used in constraints in puzzle data.
		static constexpr auto personTypeName = "person";

		std::vector<std::string> persons; // example: "Spaniard"
		std::vector<AttributeDescription> attributes; // example: "house_color" -> {"red", "green", "blue"},  "house_position" -> {"1", "2", "3"}
		std::vector<std::variant<Fact, Comparison>> constraints;
	};

	void normalize(PuzzleData&);
	void validate(const PuzzleData&); // throw PuzzleDataLogicError

	std::ostream& operator<<(std::ostream&, const PuzzleData&);
}
