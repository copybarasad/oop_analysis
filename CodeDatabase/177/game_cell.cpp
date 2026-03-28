#include "game_cell.h"

GameCell::GameCell(CellType cellType) : type(cellType), isOccupied(false) {}

CellType GameCell::getType() const {
    return type;
}

void GameCell::setType(CellType cellType) {
    type = cellType;
}

bool GameCell::getIsOccupied() const {
    return isOccupied;
}

void GameCell::setIsOccupied(bool occupied) {
    isOccupied = occupied;
}

bool GameCell::isPassable() const {
    return type != CellType::OBSTACLE;
}