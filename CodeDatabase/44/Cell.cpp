#include <stdexcept>
#include "Cell.h"

Cell::Cell() : type_(CellType::EMPTY) {}

CellType Cell::GetType() const {
    return type_;
}

void Cell::SetType(CellType new_type) {
    switch (new_type) {
        case CellType::EMPTY:
        case CellType::ENEMY:
        case CellType::PLAYER:
        case CellType::WALL:
        case CellType::SLOW:
        case CellType::BUILDING:
        case CellType::TRAP:
        case CellType::TOWER:
            type_ = new_type;
            break;
        default:
            throw std::invalid_argument("Invalid CellType");
    }
}