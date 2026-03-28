#include "Cell.h"

Cell::Cell(CellType cellType) : type(cellType), hasPlayer(false), hasEnemy(false), 
                               hasBuilding(false), hasTrap(false), hasAlly(false) {}

CellType Cell::getType() const {
    return type;
}

void Cell::setType(CellType cellType) {
    type = cellType;
}

bool Cell::isPassable() const {
    return type != CellType::IMPASSABLE && !hasBuilding && !hasAlly;
}

bool Cell::hasPlayerOnCell() const {
    return hasPlayer;
}

bool Cell::hasEnemyOnCell() const {
    return hasEnemy;
}

bool Cell::hasBuildingOnCell() const {
    return hasBuilding;
}

bool Cell::hasTrapOnCell() const {
    return hasTrap;
}

bool Cell::hasAllyOnCell() const {
    return hasAlly;
}

void Cell::setPlayerPresence(bool presence) {
    hasPlayer = presence;
}

void Cell::setEnemyPresence(bool presence) {
    hasEnemy = presence;
}

void Cell::setBuildingPresence(bool presence) {
    hasBuilding = presence;
}

void Cell::setTrapPresence(bool presence) {
    hasTrap = presence;
}

void Cell::setAllyPresence(bool presence) {
    hasAlly = presence;
}