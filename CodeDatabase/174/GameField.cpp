#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBarrack.h"
#include "EnemyTower.h"
#include "AllyUnit.h"
#include "Constants.h"
#include <iostream>
#include <chrono>
#include <random>

using namespace std;

GameField::GameField(int size) : fieldSize(size) {
    if (fieldSize < 10 || fieldSize > 25) {
        throw invalid_argument("Field size must be between 10 and 25");
    }
    grid.resize(fieldSize, vector<Cell>(fieldSize));
    initializeField();
}

GameField::GameField(const GameField& other) : fieldSize(other.fieldSize) {
    grid = other.grid;
}

GameField::GameField(GameField&& other) : fieldSize(other.fieldSize), grid(move(other.grid)) {
    other.fieldSize = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        fieldSize = other.fieldSize;
        grid = other.grid;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) {
    if (this != &other) {
        fieldSize = other.fieldSize;
        grid = move(other.grid);
        other.fieldSize = 0;
        other.grid.clear();
    }
    return *this;
}

void GameField::initializeField() {
    mt19937 generator((time(nullptr)));
    uniform_int_distribution<int> distribution(0, fieldSize - 1);

    int obstacleCount = (fieldSize * fieldSize * OBSTACLE_PERCENTAGE);
    int slowCount = (fieldSize * fieldSize * SLOW_PERCENTAGE);

    placeObstacles(obstacleCount, distribution, generator);
    placeSlowTerrain(slowCount, distribution, generator);
}

void GameField::placeObstacles(int obstacleCount, uniform_int_distribution<int>& distribution, mt19937& generator) {
    for (int i = 0; i < obstacleCount; i++) {
        int x = distribution(generator);
        int y = distribution(generator);

        if (grid[y][x].isEmpty()) {
            grid[y][x].setType(CellType::OBSTACLE);
        }
    }
}

void GameField::placeSlowTerrain(int slowCount, uniform_int_distribution<int>& distribution, mt19937& generator) {
    for (int i = 0; i < slowCount; i++) {
        int x = distribution(generator);
        int y = distribution(generator);

        if (grid[y][x].isEmpty()) {
            grid[y][x].setType(CellType::SLOW);
        }
    }
}

int GameField::getSize() const { return fieldSize; }

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < fieldSize && y >= 0 && y < fieldSize;
}

bool GameField::isValidPosition(const Position& position) const {
    return isValidPosition(position.getX(), position.getY());
}

CellType GameField::getCellType(int x, int y) const {
    if (isValidPosition(x, y)) {
        return grid[y][x].getType();
    } else {
        return CellType::EMPTY;
    }
}

bool GameField::isObstacleAt(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].isObstacle();
}

bool GameField::isSlowAt(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].isSlow();
}

bool GameField::isEmptyAt(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].isEmpty();
}

void GameField::setCellType(int x, int y, CellType type) {
    if (isValidPosition(x, y)) {
        grid[y][x].setType(type);
    }
}

void GameField::clearUnitPositions() {
    for (int y = 0; y < fieldSize; y++) {
        for (int x = 0; x < fieldSize; x++) {
            CellType currentType = grid[y][x].getType();
            if (currentType == CellType::PLAYER ||
                currentType == CellType::ENEMY ||
                currentType == CellType::ALLY ||
                currentType == CellType::ENEMY_BARRACK ||
                currentType == CellType::ENEMY_TOWER) {   
                grid[y][x].clear();
            }
        }
    }
}

bool GameField::triggerTrapAt(int x, int y, Unit& unit) {
    if (!isValidPosition(x, y)) return false;
    if (grid[y][x].getType() == CellType::TRAP) {
        unit.takeDamage(DEFAULT_TRAP_DAMAGE);
        grid[y][x].setType(CellType::EMPTY);
        return true;
    }
    return false;
}

void GameField::updateField(const Player& player,
                           const vector<shared_ptr<Enemy>>& enemies,
                           const vector<shared_ptr<EnemyBuilding>>& buildings,
                           const vector<shared_ptr<AllyUnit>>& allies) {
    clearUnitPositions();

    Position playerPos = player.getPosition();
    if (isValidPosition(playerPos)) {
        setCellType(playerPos.getX(), playerPos.getY(), player.getCellType());
    }

    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            Position enemyPos = enemy->getPosition();
            if (isValidPosition(enemyPos)) {
                setCellType(enemyPos.getX(), enemyPos.getY(), enemy->getCellType());
            }
        }
    }

    for (const auto& building : buildings) {
        if (building && building->isAlive()) {
            Position buildingPos = building->getPosition();
            if (isValidPosition(buildingPos)) {
                setCellType(buildingPos.getX(), buildingPos.getY(), building->getCellType());
            }
        }
    }

    for (const auto& ally : allies) {
        if (ally && ally->isAlive()) {
            Position allyPos = ally->getPosition();
            if (isValidPosition(allyPos)) {
                setCellType(allyPos.getX(), allyPos.getY(), ally->getCellType());
            }
        }
    }
}
