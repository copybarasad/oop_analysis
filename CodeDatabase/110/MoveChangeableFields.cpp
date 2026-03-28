#include "MoveChangeableFields.hpp"

MoveFields::MoveFields (Field& field, std::pair<size_t, size_t> target) :
    field(field), target(target) {}

Field& MoveFields::getField () const { return field; }

std::pair<size_t, size_t> MoveFields::getTarget () const { return target; }