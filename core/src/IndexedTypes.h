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


    // Some value of some attribute
    struct Attribute
    {
        AttributeTypeID typeId;
        AttributeValueID valueId;
    };

    // Fact type: the specified person has the specified attribute
    struct PersonProperty
    {
        PersonID person;
        Attribute attr;
        bool negate = false;
    };

    // Fact type: two different attributes have the same owner
    struct SameOwner
    {
        Attribute first;
        Attribute second;
        bool secondNegate = false;
    };

    // PositionComparison: сompare two persons by position in the third attribute.
    struct PositionComparison
    {
        std::variant<PersonID, Attribute> first;
        std::variant<PersonID, Attribute> second;
        AttributeTypeID compareByType;
        PuzzleDefinition::Comparison::Relation relation;
    };

    using ConstraintModel = std::variant<PersonProperty, SameOwner, PositionComparison>;
}
