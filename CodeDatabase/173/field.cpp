#include <stdexcept>

#include "field.hpp"
#include "generic-field.hpp"
#include "slime-field.hpp"
#include "wall-field.hpp"

Field *Field::fromByte(std::byte b) {
    int fieldType = static_cast<int>(b);

    switch (fieldType) {
    case GENERIC_FIELD_BYTE:
        return new GenericField();
    case SLIME_FIELD_BYTE:
        return new SlimeField();
    case WALL_FIELD_BYTE:
        return new WallField();
    default:
        throw std::logic_error("unknown field type");
    }
}

Field::~Field() {}
