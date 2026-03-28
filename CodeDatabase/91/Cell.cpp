#include "Cell.h"
#include <stdexcept>

Cell::Cell(int x, int y, bool passable, CellType cellType) : position(x, y), trapDamage(0) {
    if (cellType != CellType::EMPTY) {
        type = cellType;
    } else {
        type = passable ? CellType::EMPTY : CellType::WALL;
    }

    isPassable = (type != CellType::WALL && type != CellType::TOWER) && passable;
}

// === Getters ===
Position Cell::getPosition() const{
    return position;
}

bool Cell::getIsPassable() const {
    return isPassable;
}

CellType Cell::getType() const {
    return type;
}

int Cell::getTrapDamage() const {
    return trapDamage;
}

// === Setters ===
void Cell::setIsPassable(bool passable) {
    if (type == CellType::WALL || type == CellType::TOWER) {
        isPassable = false;
    } else {
        isPassable = passable;
    }
}

void Cell::setType(CellType cellType) {
    type = cellType;

    if (cellType == CellType::WALL || cellType == CellType::TOWER) {
        isPassable = false;
    }
    if (cellType == CellType::EMPTY) {

        isPassable = true;
    }
    if (cellType == CellType::TRAP) {
        isPassable = true;
    }
}

void Cell::setTrapDamage(int damage) {
    trapDamage = damage;
    if (damage > 0) {
        setType(CellType::TRAP);
    } else {
        if (type == CellType::TRAP) {
            setType(CellType::EMPTY);
        }
    }
}

bool Cell::hasTrap() const {
    return type == CellType::TRAP && trapDamage > 0;
}

bool Cell::hasTower() const {
    return type == CellType::TOWER;
}

void Cell::activateTrap() {
    if (hasTrap()) {
        trapDamage = 0;
        setType(CellType::EMPTY);
    }
}