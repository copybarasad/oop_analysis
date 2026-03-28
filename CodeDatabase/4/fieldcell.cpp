#include "fieldcell.hpp"

FieldCell::FieldCell(Type temp_type)
    : type(temp_type), entity(ZERO), id(0) {}

FieldCell::FieldCell(Type temp_type, EntityType temp_entity, int number)
    : type(temp_type), entity(temp_entity), id(number) {}

EntityType FieldCell::get_entity() const {
    return entity;
}

Type FieldCell::get_type() const {
    return type;
}