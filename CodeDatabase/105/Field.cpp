#include "Field.h"

#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>

// Конструктор с размерами
GameField::GameField(int w, int h) 
    : width(std::max(10, std::min(25, w))), 
      height(std::max(10, std::min(25, h))),
      playerX(0), playerY(0),
      combatManager(enemies, enemyPositions, grid, width, height) {
    initializeGrid();
}

// Конструктор копирования
GameField::GameField(const GameField& other)
    : width(other.width), height(other.height),
      playerX(other.playerX), playerY(other.playerY),
      enemyPositions(other.enemyPositions), enemies(other.enemies),
      buildingPositions(other.buildingPositions), buildings(other.buildings),
      combatManager(enemies, enemyPositions, grid, width, height) {
    
    grid.resize(height);
    for (int i = 0; i < height; i++) {
        grid[i] = other.grid[i];
    }
}

// Конструктор перемещения
GameField::GameField(GameField&& other) noexcept 
    : width(other.width), height(other.height),
      playerX(other.playerX), playerY(other.playerY),
      enemyPositions(std::move(other.enemyPositions)),
      enemies(std::move(other.enemies)),
      buildingPositions(std::move(other.buildingPositions)),
      buildings(std::move(other.buildings)),
      grid(std::move(other.grid)),
      combatManager(enemies, enemyPositions, grid, width, height) {
    
    other.width = 0;
    other.height = 0;
    other.playerX = 0;
    other.playerY = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        playerX = other.playerX;
        playerY = other.playerY;
        enemyPositions = other.enemyPositions;
        enemies = other.enemies;
        buildingPositions = other.buildingPositions;
        buildings = other.buildings;
        
        grid.resize(height);
        for (int i = 0; i < height; i++) {
            grid[i] = other.grid[i];
        }
    }
    return *this;
}


GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        playerX = other.playerX;
        playerY = other.playerY;
        enemyPositions = std::move(other.enemyPositions);
        enemies = std::move(other.enemies);
        buildingPositions = std::move(other.buildingPositions);
        buildings = std::move(other.buildings);
        grid = std::move(other.grid);
        
        other.width = 0;
        other.height = 0;
        other.playerX = 0;
        other.playerY = 0;
    }
    return *this;
}


void GameField::initializeGrid() {
    grid.resize(height, std::vector<Cell>(width));
    
    std::mt19937 gen(std::time(nullptr));
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    // Заполняем поле разными типами клеток
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double randVal = dis(gen);
            if (randVal < 0.1) {
                grid[y][x].setType(CellType::BLOCKED);
            } else if (randVal < 0.2) {
                grid[y][x].setType(CellType::SLOW);
            }
        }
    }
    
    // Устанавливаем игрока в начальную позицию
    playerX = 0;
    playerY = 0;
    grid[0][0].setPlayerPresence(true);
    
    // Добавляем несколько врагов
    addRandomEnemies(1);
    
    // Добавляем здания
    addRandomBuildings(1);
}

void GameField::addRandomEnemies(int count) {
    std::mt19937 gen(std::time(nullptr));
    std::uniform_int_distribution<> xDist(0, width - 1);
    std::uniform_int_distribution<> yDist(0, height - 1);
    
    for (int i = 0; i < count; i++) {
        int x, y;
        do {
            x = xDist(gen);
            y = yDist(gen);
        } while (!isCellPassable(x, y) || hasPlayer(x, y) || hasEnemy(x, y) || hasBuilding(x, y));
        
        enemyPositions.emplace_back(x, y);
        enemies.emplace_back();
        grid[y][x].setEnemyPresence(true);
    }
}

void GameField::addRandomBuildings(int count) {
    std::mt19937 gen(std::time(nullptr));
    std::uniform_int_distribution<> xDist(0, width - 1);
    std::uniform_int_distribution<> yDist(0, height - 1);
    std::uniform_int_distribution<> intervalDist(10, 15);
    
    for (int i = 0; i < count; i++) {
        int x, y;
        do {
            x = xDist(gen);
            y = yDist(gen);
        } while (!isCellPassable(x, y) || hasPlayer(x, y) || hasEnemy(x, y) || hasBuilding(x, y));
        
        buildingPositions.emplace_back(x, y);
        buildings.emplace_back(intervalDist(gen));
        grid[y][x].setBuildingPresence(true);
        grid[y][x].setType(CellType::BLOCKED);
    }
}


bool GameField::setPlayerPosition(int x, int y, Player& player) {
    if (x < 0 || x >= width || y < 0 || y >= height || !isCellPassable(x, y)) {
        return false;
    }
    
    // Если на клетке враг - атакуем его
    if (hasEnemy(x, y)) {
        return attackEnemy(x, y, player);
    }

    // Убираем игрока со старой позиции
    if (playerX >= 0 && playerY >= 0 && playerX < width && playerY < height) {
        grid[playerY][playerX].setPlayerPresence(false);
    }
    
    // Устанавливаем на новую позицию
    playerX = x;
    playerY = y;
    grid[y][x].setPlayerPresence(true);
    
    return true;
}

std::pair<int, int> GameField::getPlayerPosition() const {
    return {playerX, playerY};
}


bool GameField::isCellPassable(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return grid[y][x].isPassable();
}

bool GameField::hasPlayer(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return grid[y][x].hasPlayerHere();
}

bool GameField::hasEnemy(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return grid[y][x].hasEnemyHere();
}

bool GameField::hasBuilding(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return grid[y][x].hasBuildingHere();
}

CellType GameField::getCellType(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return CellType::BLOCKED;
    return grid[y][x].getType();
}

// Метод для спавна врагов из зданий
void GameField::spawnEnemiesFromBuildings() {
    for (size_t i = 0; i < buildingPositions.size(); i++) {
        if (buildings[i].shouldSpawnEnemy()) {
            int bx = buildingPositions[i].first;
            int by = buildingPositions[i].second;
            
            // Ищем свободную клетку вокруг здания
            std::vector<std::pair<int, int>> possiblePositions;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = bx + dx;
                    int ny = by + dy;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && 
                        isCellPassable(nx, ny) && !hasEnemy(nx, ny) && !hasPlayer(nx, ny)) {
                        possiblePositions.emplace_back(nx, ny);
                    }
                }
            }
            
            if (!possiblePositions.empty()) {
                std::mt19937 gen(std::time(nullptr));
                std::uniform_int_distribution<> posDist(0, possiblePositions.size() - 1);
                
                auto pos = possiblePositions[posDist(gen)];
                enemyPositions.emplace_back(pos.first, pos.second);
                enemies.emplace_back();
                grid[pos.second][pos.first].setEnemyPresence(true);
            }
        }
    }
}

// Добавляем в Field.cpp
void GameField::clearAll() {
    enemies.clear();
    enemyPositions.clear();
    buildings.clear();
    buildingPositions.clear();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x].setState(CellType::EMPTY, false, false, false);
        }
    }
}

void GameField::setCellType(int x, int y, CellType type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x].setType(type);
    }
}

void GameField::placeEnemy(int x, int y, const Enemy& enemy) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        enemies.push_back(enemy);
        enemyPositions.push_back({x, y});
        grid[y][x].setEnemyPresence(true);
    }
}

void GameField::placeBuilding(int x, int y, const EnemyBuilding& building) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        buildings.push_back(building);
        buildingPositions.push_back({x, y});
        grid[y][x].setBuildingPresence(true);
    }
}

void GameField::setInternalPlayerPosition(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        // Сбрасываем старую позицию
        if (playerX >= 0 && playerY >= 0) {
            grid[playerY][playerX].setPlayerPresence(false);
        }
        
        playerX = x;
        playerY = y;
        grid[playerY][playerX].setPlayerPresence(true);
    }
}

