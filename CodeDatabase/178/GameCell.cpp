#include "GameCell.h"

GameCell::GameCell(CellType cellType) : type(cellType), isOccupied(false) {}

CellType GameCell::getType() const { return type; }
bool GameCell::getIsOccupied() const { return isOccupied; }
bool GameCell::isPassable() const { return type != CellType::OBSTACLE && !isOccupied; }