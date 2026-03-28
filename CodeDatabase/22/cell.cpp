#include "field/cell.h"

// Constructor
Cell::Cell(Type type, Ocupant ocup)
    : typeCell(type), cellOcupant(ocup), readyToUse(false) {}

// getter
Type Cell::getType() { return typeCell; }
Type Cell::getType() const
{
    return typeCell;
}
Ocupant Cell::getOcupant() { return cellOcupant; }
Ocupant Cell::getOcupant() const
{
    return cellOcupant;
}

// setters
void Cell::setType(Type type) { typeCell = type; }
void Cell::setOcupant(Ocupant ocupant) { cellOcupant = ocupant; }

int Cell::string(){
    if (typeCell == SLOW) return 0;
    else if (typeCell == WALL) return 1;
    else if (typeCell == SPACE) return 2;
    else if (typeCell == TRAP) return 3;

    return -1;
}

//spells
void Cell::setReadyToUse(bool rtu){this->readyToUse = rtu;}
bool Cell::getReady(){return readyToUse;}
bool Cell::getReady() const { return readyToUse; }
