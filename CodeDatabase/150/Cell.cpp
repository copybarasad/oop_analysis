#include "Cell.hpp"

Cell::Cell(CellType type) 
    : type_(type), hasPlayer_(false), hasBuilding_(false), hasEnemy_(false) {}

CellType Cell::getType() const {
    return type_;
}

void Cell::setType(CellType type) {
    type_ = type;
}

bool Cell::isEmpty() const {
    return !hasPlayer_ && !hasEnemy_ && !hasBuilding_ && !hasAlly_;
}

bool Cell::isPassable() const {
    return type_ != CellType::Impassable;
}

bool Cell::isSlowing() const {
    return type_ == CellType::Slowing;
}

bool Cell::hasPlayer() const {
    return hasPlayer_;
}

void Cell::setHasPlayer(bool hasPlayer) {
    hasPlayer_ = hasPlayer;
}

bool Cell::hasEnemy() const {
    return hasEnemy_;
}

void Cell::setHasEnemy(bool hasEnemy) {
    hasEnemy_ = hasEnemy;
}

bool Cell::hasBuilding() const {
    return hasBuilding_;
}

void Cell::setHasBuilding(bool hasBuilding) {
    hasBuilding_ = hasBuilding;
}

bool Cell::hasAlly() const {
    return hasAlly_;
}

void Cell::setHasAlly(bool hasAlly) {
    hasAlly_ = hasAlly;
}

void Cell::clear() {
    hasPlayer_ = false;
    hasEnemy_ = false;
    hasBuilding_ = false;
    hasAlly_ = false;
}