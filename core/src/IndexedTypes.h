#pragma once
#include "epuzzle/PuzzleDefinition.h"

namespace epuzzle::details
{
    using AttributeTypeID = utils::TypedIndex<struct AttributeTypeID_tag>;
    using AttributeValueID = utils::TypedIndex<struct AttributeValueID_tag>;
    using PersonID = utils::TypedIndex<struct PersonID_tag>;

    // Combination of values of one attribute. For example, for attribute type "house_color":
    // AttributeAssignment[red] = Alice, AttributeAssignment[green] = Bob, AttributeAssignment[white] = John, ...)
    using AttributeAssignment = utils::IndexedVector<AttributeValueID, PersonID>; 

    // Note: person - special attribute type (not so pretty, but very practical)
    static constexpr auto AttributeTypeID_person = AttributeTypeID{ std::numeric_limits<size_t>::max() };

    using Relation = PuzzleDefinition::Comparison::Relation;

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

    using ConstraintModel = std::variant<PersonProperty, SameOwner, PositionComparison>;
}
