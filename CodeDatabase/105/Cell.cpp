#include "Cell.h"

Cell::Cell() : type(CellType::EMPTY), hasPlayer(false), hasEnemy(false), hasBuilding(false) {}

CellType Cell::getType() const { return type; }
void Cell::setType(CellType newType) { type = newType; }

bool Cell::hasPlayerHere() const { return hasPlayer; }
void Cell::setPlayerPresence(bool presence) { hasPlayer = presence; }

bool Cell::hasEnemyHere() const { return hasEnemy; }
void Cell::setEnemyPresence(bool presence) { hasEnemy = presence; }

bool Cell::hasBuildingHere() const { return hasBuilding; }
void Cell::setBuildingPresence(bool presence) { hasBuilding = presence; }

bool Cell::isPassable() const { return type != CellType::BLOCKED; }

void Cell::setState(CellType newType, bool player, bool enemy, bool building) {
    type = newType;
    hasPlayer = player;
    hasEnemy = enemy;
    hasBuilding = building;
}


