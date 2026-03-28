#include "EnemyBuilding.h"
#include <vector>
#include <algorithm>
#include <random>
#include <functional>

EnemyBuilding::EnemyBuilding(int x, int y, int cooldown, int maxX, int maxY)
    : positionX(x), positionY(y), spawnCooldown(cooldown), currentCooldown(cooldown) {
    validateCooldown(cooldown);

    if (x < 0 || y < 0 || x >= maxX || y >= maxY) {
        throw std::invalid_argument("Координаты здания вне поля");
    }
}

void EnemyBuilding::getPosition(int &x, int &y) const {
    x = positionX;
    y = positionY;
}

bool EnemyBuilding::canSpawnEnemy() const {
    return currentCooldown <= 0;
}

void EnemyBuilding::update() {
    if (currentCooldown > 0) {
        --currentCooldown;
    }
}

void EnemyBuilding::resetCooldown() {
    currentCooldown = spawnCooldown;
}

bool EnemyBuilding::trySpawnEnemy(int &spawnX, int &spawnY,
                                  std::function<bool(int, int)> isPositionFree) const {
    std::vector<std::pair<int, int> > directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    std::vector<std::pair<int, int> > shuffledDirections = directions;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffledDirections.begin(), shuffledDirections.end(), g);

    for (const auto &dir: shuffledDirections) {
        int newX = positionX + dir.first;
        int newY = positionY + dir.second;

        if (isPositionFree(newX, newY)) {
            spawnX = newX;
            spawnY = newY;
            return true;
        }
    }

    return false;
}

int EnemyBuilding::getSpawnCooldown() const {
    return spawnCooldown;
}

int EnemyBuilding::getCurrentCooldown() const {
    return currentCooldown;
}
