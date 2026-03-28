#include "Cell.h"

Cell::Cell() : type(CellType::Empty) {}

void Cell::setType(CellType t) { 
    type = t; 
}

CellType Cell::getType() const { 
    return type; 
}

bool Cell::isEmpty() const {
    return type == CellType::Empty;
}

bool Cell::isWall() const {
    return type == CellType::Wall;
}

bool Cell::isOccupied() const {
    return type == CellType::Player || type == CellType::Enemy;
}