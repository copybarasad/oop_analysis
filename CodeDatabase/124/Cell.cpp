#include "cell.h"

Cell::Cell() : isOccupied(false), hasPlayer(false), hasEnemy(false), isPassable(true) {}

bool Cell::getIsOccupied() const { return isOccupied; }
bool Cell::getHasPlayer() const { return hasPlayer; }
bool Cell::getHasEnemy() const { return hasEnemy; }
bool Cell::getIsPassable() const { return isPassable; }

void Cell::setHasPlayer(bool value) {
    hasPlayer = value;
    isOccupied = hasPlayer || hasEnemy;
}

void Cell::setHasEnemy(bool value) {
    hasEnemy = value;
    isOccupied = hasPlayer || hasEnemy;
}

void Cell::setIsPassable(bool value) { isPassable = value; }

void Cell::clear() {
    hasPlayer = false;
    hasEnemy = false;
    isOccupied = false;
    isPassable = true;
}