#include "EnemyBarrack.h"
#include "GameField.h"
#include "Constants.h"
#include <random>
#include <chrono>

EnemyBarrack::EnemyBarrack(int x, int y, int cooldown)
    : EnemyBuilding(x, y, DEFAULT_BARRACK_HEALTH, DEFAULT_BARRACK_DAMAGE),
      spawnInterval(cooldown), currentInterval(cooldown) {}

bool EnemyBarrack::canSpawnEnemy() {
    if (currentInterval <= 0) {
        currentInterval = spawnInterval;
        return true;
    }
    currentInterval--;
    return false;
}

Position EnemyBarrack::getSpawnPosition(const GameField& field) const {
    std::vector<Position> possiblePositions;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int newX = position.getX() + dx;
            int newY = position.getY() + dy;

            if (field.isValidPosition(newX, newY) &&
                !field.isObstacleAt(newX, newY) &&
                field.isEmptyAt(newX, newY)) {
                possiblePositions.push_back(Position(newX, newY));
            }
        }
    }

    if (possiblePositions.empty()) {
        return Position(-1, -1);
    }

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(0, possiblePositions.size() - 1);
    return possiblePositions[distribution(generator)];
}

CellType EnemyBarrack::getCellType() const {
    return CellType::ENEMY_BARRACK;
}