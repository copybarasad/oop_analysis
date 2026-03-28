#include "Cell.h"
#include <iostream>

Cell::Cell(Type_cell cellType, bool canWalk) 
    : type(cellType), can_go(canWalk) {}

Type_cell Cell::getType() const { return type; }
bool Cell::isWalkable() const { return can_go; }

void Cell::setType(Type_cell newType) { type = newType; }
void Cell::setWalkable(bool canWalk) { can_go = canWalk; }

char Cell::getSymbol() const {
    switch(type) {
        case EMPTY: return '.';
        case PLAYER: return 'P';
        case ENEMY: return 'E';
        case PIT: return 'O';
        case BARRACK: return 'B';
        case SALOON: return 'S';
        case COWBOYTOWER: return 'T';
        case TRAP: return '+';
        case JHONNYGHOST: return 'J';
        default: return '?';
    }
}