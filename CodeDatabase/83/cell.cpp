#include "cell.h"

Cell::Cell() : passable(true) {}

bool Cell::canPass() const {
    return passable;
}

void Cell::setPassable(bool canPass) {
    passable = canPass;
}