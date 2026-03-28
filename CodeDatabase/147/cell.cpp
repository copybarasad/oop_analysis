#include "cell.h"
#include "unit.h"

Cell::Cell() : type(CellType::EMPTY), occupant(nullptr) {}

Cell::Cell(CellType t) : type(t), occupant(nullptr) {}

void Cell::setType(CellType newType) {
    // Не разрешаем менять terrain, если в клетке есть occupant (инвариант)
    if (occupant != nullptr) {
        return;
    }
    type = newType;
}

CellType Cell::getType() const {
    return type;
}

void Cell::setOccupant(Unit* unit) {
    occupant = unit;
    if (occupant != nullptr) {
        type = occupant->get_type();
    } else {
        type = CellType::EMPTY;
    }
}

void Cell::clearOccupant() {
    occupant = nullptr;
    type = CellType::EMPTY;
}

Unit* Cell::getOccupant() const {
    return occupant;
}

bool Cell::hasOccupant() const {
    return occupant != nullptr;
}

bool Cell::isEmpty() const {
    return occupant == nullptr && type == CellType::EMPTY;
}

bool Cell::isPassable() const {
    // Если есть occupant, клетка непроходима
    if (occupant != nullptr) return false;
    // Проходимы только пустые клетки и замедляющие
    return type == CellType::EMPTY || type == CellType::FREEZE;
}
