#include "Cell.h"

Cell::Cell(CellType cellType) : type(cellType) {
    passable = (cellType != CellType::WALL);
}

void Cell::setType(CellType cellType) {
    type = cellType;
    passable = (cellType != CellType::WALL);
}

char Cell::getSymbol() const {
    switch(type) {
        case CellType::WALL: return '#';
        case CellType::PLAYER: return 'P';
        case CellType::ENEMY: return 'E';
        default: return '.';
    }
}
