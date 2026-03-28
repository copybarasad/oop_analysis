// Cell.cpp
#include "Cell.h"

Cell::Cell() : type_(Type::Empty) {}
Cell::Cell(Type t) : type_(t) {}

Cell::Type Cell::getType() const { return type_; }
void Cell::setType(Type t) { type_ = t; }

char Cell::toChar() const {
    return (type_ == Type::Impassable) ? '#' : '.';
}