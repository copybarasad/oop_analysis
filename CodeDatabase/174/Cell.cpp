#include "Cell.h"

Cell::Cell() : cellType(CellType::EMPTY) {}

CellType Cell::getType() const { return cellType; }
void Cell::setType(CellType newType) { cellType = newType; }

bool Cell::isEmpty() const { return cellType == CellType::EMPTY; }
bool Cell::isObstacle() const { return cellType == CellType::OBSTACLE; }
bool Cell::isSlow() const { return cellType == CellType::SLOW; }

void Cell::clear() { 
    cellType = CellType::EMPTY; 
}