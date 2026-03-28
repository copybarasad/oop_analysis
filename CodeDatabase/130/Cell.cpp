#include "Cell.h"

Cell::Cell() : type(CellType::EMPTY), hasEnemy(false), hasBuilding(false) {}
Cell::Cell(CellType type) : type(type), hasEnemy(false), hasBuilding(false) {}

CellType Cell::getType() const { return type; }
bool Cell::hasEnemyOnCell() const { return hasEnemy; }
bool Cell::hasBuildingOnCell() const { return hasBuilding; }
bool Cell::isEmpty() const { return !hasEnemy && !hasBuilding; }
bool Cell::isPassable() const { return type != CellType::BLOCKED; }

void Cell::setType(CellType newType) { type = newType; }
void Cell::setEnemyPresence(bool presence) { hasEnemy = presence; }
void Cell::setBuildingPresence(bool presence) { hasBuilding = presence; }

void Cell::clearEntities() {
    hasEnemy = false;
    hasBuilding = false;
}