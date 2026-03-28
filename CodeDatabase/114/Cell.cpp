#include "Cell.h"

Cell::Cell() : type(CellType::EMPTY), trapDamage(TRAP_DAMAGE), hasEntity(false) {
}

void Cell::setType(CellType newType) {
    type = newType;
}

void Cell::setHasEntity(bool hasEntity) {
    this->hasEntity = hasEntity;
}

void Cell::setTrapDamage(int trapDamage) {
    this->trapDamage = trapDamage;
}

bool Cell::canMoveTo() const {
    return type != CellType::WALL && !hasEntity;
}
