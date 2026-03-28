#include "cell.h"

Cell::Cell()
    : type(CellType::EMPTY), hasTrapDamage(0){

}

CellType Cell::getType() const { 
    return type; 
}

int Cell::getTrapDamage() const {
    return hasTrapDamage;
}

bool Cell::isPassable() const {
    return type != CellType::WALL;
}

void Cell::setTrapDamage(int dmg) {
    hasTrapDamage = dmg;
}
void Cell::setType(CellType newType) {
    if (type == CellType::TRAP && newType != CellType::TRAP){
        hasTrapDamage = 0;
    }
    type = newType;
}