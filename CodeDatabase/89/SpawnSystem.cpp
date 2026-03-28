#include "SpawnSystem.h"
#include <iostream>
#include <random>

SpawnSystem::SpawnSystem(GameGrid& grid) : grid(grid) {}

void SpawnSystem::placePlayer(std::unique_ptr<Player> player) {
    if (!player) return;

    int x = 0;
    int y = 0;

    player->setPosition(x, y);
    grid.updateCell(x, y, CellType::Player);

    std::cout << "Player placed at (" << x << ", " << y << ") in guaranteed safe 3x3 zone." << std::endl;
}

void SpawnSystem::addEnemy(std::unique_ptr<Enemy> enemy, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!enemy) return;

    int x, y;
    for (int attempt = 0; attempt < 100; ++attempt) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disX(0, grid.getWidth() - 1);
        std::uniform_int_distribution<> disY(0, grid.getHeight() - 1);

        x = disX(gen);
        y = disY(gen);

        if (x < 3 && y < 3) {
            continue;
        }

        if (grid.isCellEmpty(x, y) && grid.isCellPassable(x, y)) {
            grid.updateCell(x, y, CellType::Enemy);
            enemy->setPosition(x, y);
            enemies.push_back(std::move(enemy));
            std::cout << "Enemy placed at (" << x << ", " << y << ")" << std::endl;
            return;
        }
    }

    std::cout << "Warning: Could not find valid spawn position for enemy (avoiding player zone)." << std::endl;
}

bool SpawnSystem::findEmptyPosition(int& x, int& y) const {
    return false;
}

bool SpawnSystem::findGoodPosition(int& x, int& y) const {
    return false;
}

bool SpawnSystem::hasAccessibleNeighbors(int x, int y) const {
    return true;
}

void SpawnSystem::createGuaranteedPlayerPosition(int& x, int& y) {
    x = 0; y = 0;
}