#include "GameGrid.h"
#include <random>
#include <iostream>
#include <ctime>

void GameGrid::setPlayerPosition(int x, int y) {
    if (isPositionValid(x, y) && isCellPassable(x, y)) {
        int oldX, oldY;
        if (player) {
            player->getPosition(oldX, oldY);
            cells[oldY][oldX].setPlayerPresence(false);
        }
        player->setPosition(x, y);
        cells[y][x].setPlayerPresence(true);
    }
}

void GameGrid::setCellTerrain(int x, int y, TerrainType type) {
    if (isPositionValid(x, y)) {
        cells[y][x].setTerrain(type);
    }
}

void GameGrid::clearAll() {
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            cells[y][x].setPlayerPresence(false);
            cells[y][x].setEnemyPresence(false);
            cells[y][x].setStructurePresence(false);
        }
    }

    enemies.clear();
    enemyPositions.clear();
    structures.clear();
    structurePositions.clear();
}
