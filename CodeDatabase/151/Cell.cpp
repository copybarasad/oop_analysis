#include "Cell.h"

Cell::Cell() : hasPlayer(false), hasEnemy(false) {}

bool Cell::getHasPlayer() const {
    return hasPlayer;
}

bool Cell::getHasEnemy() const {
    return hasEnemy;
}

void Cell::setHasPlayer(bool value) {
    hasPlayer = value;
}

void Cell::setHasEnemy(bool value) {
    hasEnemy = value;
}

bool Cell::isEmpty() const {
    return !hasPlayer && !hasEnemy;
}

void Cell::clear() {
    hasPlayer = false;
    hasEnemy = false;
}