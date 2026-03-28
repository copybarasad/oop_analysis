#include "Cell.h"


Cell::Cell() = default;
Cell::Cell(Type type) : contains(type) {}

Cell::Type Cell::getType() const { return contains; }
void Cell::setType(Type t) { contains = t;
}

std::string Cell::to_string() const {
    switch (contains) {
        case Type::Player: return "P";
        case Type::Enemy:  return "E";
        case Type::Wall:   return "█";
        case Type::Empty:  return "•";
        case Type::Trap:   return INVISIBLE_TRAPS ? "•" : "x";
        case Type::Tower:  return "T";
        default: return "?";
    }
}