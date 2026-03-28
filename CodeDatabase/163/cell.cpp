#include "cell.hpp"
#include <string>

Cell::Cell(bool empty, Type type)
    : isEmpty(empty), cellType(type) {
}

bool Cell::isEmptyCell() const {
    return isEmpty;
}

Cell::Type Cell::getCellType() const {
    return cellType;
}

void Cell::setCellType(Type type) {
    cellType = type;
}

void Cell::setEmpty(bool empty) {
    isEmpty = empty;
}

bool Cell::isPassable() const {
    return isEmpty && cellType != Type::BLOCKING;
}

std::string Cell::typeToString(Type type) {
    switch(type) {
        case Type::COMMON: return "common";
        case Type::BLOCKING: return "blocking";
        case Type::SLOWING: return "slowing";
        default: return "common";
    }
}

Cell::Type Cell::stringToType(const std::string& typeStr) {
    if (typeStr == "blocking") return Type::BLOCKING;
    if (typeStr == "slowing") return Type::SLOWING;
    return Type::COMMON;
}