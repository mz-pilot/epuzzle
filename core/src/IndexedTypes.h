#pragma once
#include "epuzzle/PuzzleData.h"

namespace epuzzle::details
{
	using AttributeTypeID	= utils::TypedIndex<struct AttributeTypeID_tag>;
	using AttributeValueID	= utils::TypedIndex<struct AttributeValueID_tag>;
	using PersonID			= utils::TypedIndex<struct PersonID_tag>;

	using Assignment		= utils::IndexedVector<AttributeValueID, PersonID>; // Single combination values of single attribute

	// Note: person - special attribute type (not so pretty, but very practical)
	static constexpr auto AttributeTypeID_person = AttributeTypeID{std::numeric_limits<size_t>::max()};
	
	using Relation = PuzzleData::Comparison::Relation;
	
	struct Attribute
	{
		AttributeTypeID typeId;
		AttributeValueID valueId;
	};

	struct PersonProperty
	{
		PersonID person;
		Attribute attr;
		bool negate = false;
	};

	struct SameOwner
	{
		Attribute first;
		Attribute second;
		bool secondNegate = false;
	};

	struct PositionComparison
	{
		Attribute first;
		Attribute second;
		AttributeTypeID compareByType;
		Relation relation;
	};

	using IndexedConstraint = std::variant<PersonProperty, SameOwner, PositionComparison>;
}