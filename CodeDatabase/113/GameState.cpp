#include "GameState.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <iostream>
#include <random>

GameState::GameState() : m_randomGen(std::random_device{}()) {
    reset();
}

void GameState::reset() {
    turnCount = 0;
    playerHealth = 10000;
    playerGold = 100;
    level = 1;
    gameRunning = false;
    playerX = 0;
    playerY = 0;
    enemies.clear();
    towers.clear();

    field.initialize(15, 15);
}

bool GameState::initialize() {
    return initializeLevel(1);
}

bool GameState::initializeLevel(int newLevel) {
    reset();
    level = newLevel;

    int enemyHealth = 50 + (level - 1) * 20;
    int enemyDamage = 5 + (level - 1) * 2;
    int towerHealth = 100 + (level - 1) * 30;
    int towerDamage = 10 + (level - 1) * 3;

    enemies.emplace_back(enemyHealth, enemyDamage, 3, 3);
    enemies.emplace_back(enemyHealth, enemyDamage, 12, 12);
    towers.emplace_back(towerHealth, towerDamage, 5, 5, 5);
    towers.emplace_back(towerHealth, towerDamage, 5, 10, 10);

    
    initializeObstacles();

    std::cout << "Level " << level << " initialized with "
        << enemies.size() << " enemies and "
        << towers.size() << " towers" << std::endl;
    return true;
}

void GameState::initializeObstacles() {
    field.clear();

    std::uniform_int_distribution<> distX(0, field.getWidth() - 1);
    std::uniform_int_distribution<> distY(0, field.getHeight() - 1);

    int totalCells = field.getWidth() * field.getHeight();
    int obstacleCount = static_cast<int>(totalCells * 0.2);

    int placed = 0;
    int attempts = 0;
    int maxAttempts = totalCells * 3;

    while (placed < obstacleCount && attempts < maxAttempts) {
        int x = distX(m_randomGen);
        int y = distY(m_randomGen);
        attempts++;

        if (field.getCell(x, y) == ' ' &&
            !isPositionOccupied(x, y) &&
            !isNearPlayerStart(x, y)) {

            field.setCell(x, y, '#');
            placed++;
        }
    }

    std::cout << "Generated " << placed << " random obstacles for level " << level << std::endl;
}

bool GameState::isPositionOccupied(int x, int y) const {
    for (const auto& enemy : enemies) {
        if (enemy.getPositionX() == x && enemy.getPositionY() == y) {
            return true;
        }
    }

    for (const auto& tower : towers) {
        if (tower.getPositionX() == x && tower.getPositionY() == y) {
            return true;
        }
    }

    return false;
}

bool GameState::isNearPlayerStart(int x, int y) const {
    return (x >= 0 && x <= 2 && y >= 0 && y <= 2);
}