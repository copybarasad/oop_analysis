#include "../include/GameWorld.h"
#include <cmath>
#include <random>
#include <ctime>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GameWorld::GameWorld(int width, int height)
    : grid(width, height),
      playerPosition(1, 1),
      combatSystem(pathfinding) {}

int GameWorld::getWidth() const {
    return grid.getWidth();
}

int GameWorld::getHeight() const {
    return grid.getHeight();
}

Position GameWorld::getPlayerPosition() const {
    return playerPosition;
}

CellType GameWorld::getCellType(const Position& pos) const {
    return grid.getCellType(pos);
}

void GameWorld::initializeWorld() {
    playerPosition = Position(1, 1);
    
    int fieldSize = grid.getWidth();
    int totalCells = fieldSize * fieldSize;
    
    int wallCount = static_cast<int>(fieldSize * 1.5);
    int slowingCount = fieldSize / 2;
    
    bool validLayout = false;
    int attempts = 0;
    
    while (!validLayout && attempts < 10) {
        attempts++;
        
        grid.clearGrid();
        grid.generateWalls(wallCount);
        grid.generateSlowingCells(slowingCount);
        
        int reachableCells = grid.countReachableCells(playerPosition);
        int minRequiredCells = totalCells / 3;
        
        if (reachableCells >= minRequiredCells) {
            validLayout = true;
        }
    }
}

void GameWorld::spawnInitialEnemies(int count, int health, int damage) {
    int fieldSize = grid.getWidth();
    
    auto findNearestFreeCell = [this, fieldSize](int targetX, int targetY) -> Position {
        for (int radius = 0; radius < fieldSize; radius++) {
            for (int dx = -radius; dx <= radius; dx++) {
                for (int dy = -radius; dy <= radius; dy++) {
                    if (abs(dx) + abs(dy) > radius) continue;
                    
                    int x = targetX + dx;
                    int y = targetY + dy;
                    Position pos(x, y);
                    
                    if (grid.canMoveTo(pos) && pos != playerPosition) {
                        return pos;
                    }
                }
            }
        }
        return Position(fieldSize / 2, fieldSize / 2);
    };
    
    std::vector<std::vector<Direction>> patrolPatterns = {
        {Direction::RIGHT, Direction::DOWN, Direction::LEFT, Direction::UP},
        {Direction::UP, Direction::RIGHT, Direction::DOWN, Direction::LEFT},
        {Direction::LEFT, Direction::UP, Direction::RIGHT, Direction::DOWN},
        {Direction::DOWN, Direction::LEFT, Direction::UP, Direction::RIGHT},
        {Direction::RIGHT, Direction::UP, Direction::LEFT, Direction::DOWN},
        {Direction::DOWN, Direction::RIGHT, Direction::UP, Direction::LEFT},
        {Direction::LEFT, Direction::DOWN, Direction::RIGHT, Direction::UP}
    };
    
    std::vector<std::pair<int, int>> spawnLocations = {
        {fieldSize / 2, fieldSize / 2},
        {fieldSize / 3, fieldSize * 2 / 3},
        {fieldSize * 2 / 3, fieldSize / 3},
        {fieldSize / 4, fieldSize / 4},
        {fieldSize * 3 / 4, fieldSize * 3 / 4},
        {fieldSize / 2, fieldSize / 4},
        {fieldSize / 2, fieldSize * 3 / 4}
    };
    
    for (int i = 0; i < count && i < 7; i++) {
        Position pos = findNearestFreeCell(spawnLocations[i].first, spawnLocations[i].second);
        enemyManager.addEnemy(pos, std::make_unique<Enemy>(health, damage, patrolPatterns[i]));
    }
}

void GameWorld::spawnBuildingsAtDistance(double minDistance) {
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    
    int fieldSize = grid.getWidth();
    double spawnX = playerPosition.getX();
    double spawnY = playerPosition.getY();
    
    double maxDx = (fieldSize - 3) - spawnX;
    double maxDy = (fieldSize - 3) - spawnY;
    double maxRadiusFromPlayer = std::sqrt(maxDx * maxDx + maxDy * maxDy);
    
    int numBuildings = (fieldSize <= 15) ? 1 : 2;
    double minDistanceBetweenBuildings = fieldSize / 3.0;
    
    std::vector<Position> buildingPositions;
    
    if (minDistance > maxRadiusFromPlayer) {
        return;
    }
    
    if (fieldSize <= 12) {
        minDistance = std::min(minDistance, fieldSize / 2.5);
    }
    
    for (int i = 0; i < numBuildings; ++i) {
        Position buildingPos;
        int attempts = 0;
        bool validPosition = false;
        
        int maxAttempts = (fieldSize <= 12) ? 500 : 1000;
        
        while (!validPosition && attempts < maxAttempts) {
            attempts++;
            
            int x, y;
            
            {
                std::uniform_real_distribution<double> angleDist(0.0, 2.0 * M_PI);
                std::uniform_real_distribution<double> radiusDist(minDistance, maxRadiusFromPlayer);
                
                double angle = angleDist(rng);
                double radius = radiusDist(rng);
                
                x = static_cast<int>(spawnX + radius * std::cos(angle));
                y = static_cast<int>(spawnY + radius * std::sin(angle));
                
                if (x < 2 || x >= fieldSize - 2 || y < 2 || y >= fieldSize - 2) {
                    continue;
                }
                
                double actualDx = x - spawnX;
                double actualDy = y - spawnY;
                double actualDistance = std::sqrt(actualDx * actualDx + actualDy * actualDy);
                
                if (actualDistance < minDistance) {
                    continue;
                }
            }
            
            buildingPos = Position(x, y);
            
            if (buildingPos == playerPosition) {
                continue;
            }
            
            if (i > 0) {
                double dx = buildingPos.getX() - buildingPositions[0].getX();
                double dy = buildingPos.getY() - buildingPositions[0].getY();
                double distance = std::sqrt(dx * dx + dy * dy);
                
                if (distance < minDistanceBetweenBuildings) {
                    continue;
                }
            }
            
            validPosition = true;
        }
        
        if (validPosition) {
            buildingPositions.push_back(buildingPos);
            buildingManager.addBuilding(buildingPos, std::make_unique<EnemyBuilding>(5));
        }
    }
}

bool GameWorld::movePlayer(Direction direction, Player& player) {
    return combatSystem.movePlayer(direction, player, playerPosition, grid, enemyManager);
}

void GameWorld::processEnemyTurn(Player& player, int& killedEnemiesOut, int& alliesKilledOut) {
    allyManager.moveAllies(playerPosition, grid, enemyManager, enemyManager, pathfinding, buildingManager, towerManager);
    alliesKilledOut = enemyManager.removeDeadEnemies();
    
    combatSystem.moveEnemies(player, playerPosition, grid, enemyManager, buildingManager, towerManager, allyManager);
    
    for (auto& enemyPair : enemyManager.getEnemies()) {
        if (trapManager.hasTrapAt(enemyPair.first)) {
            trapManager.triggerTrap(enemyPair.first, enemyManager);
        }
    }
    
    towerManager.processTowers(player, playerPosition);
    buildingManager.processSpawns(enemyManager, grid);
    
    killedEnemiesOut = enemyManager.removeDeadEnemies();
    killedEnemiesOut += buildingManager.removeDeadBuildings();
    killedEnemiesOut += towerManager.removeDeadTowers();
    allyManager.removeDeadAllies();
}

int GameWorld::attackInDirection(Direction direction, int damage, bool isRanged, int& outTotalDamage) {
    return combatSystem.attackInDirection(direction, damage, isRanged, playerPosition, grid, enemyManager, outTotalDamage);
}

bool GameWorld::hasAliveEnemyAt(const Position& pos) const {
    return enemyManager.hasAliveEnemyAt(pos);
}

bool GameWorld::hasBuildingAt(const Position& pos) const {
    return buildingManager.hasBuildingAt(pos);
}

int GameWorld::removeDeadEnemies() {
    int killed = enemyManager.removeDeadEnemies();
    killed += buildingManager.removeDeadBuildings();
    killed += towerManager.removeDeadTowers();
    return killed;
}

void GameWorld::spawnTowersAtDistance(double minDistance) {
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)) + 123);
    
    int fieldSize = grid.getWidth();
    double spawnX = playerPosition.getX();
    double spawnY = playerPosition.getY();
    
    double maxDx = (fieldSize - 3) - spawnX;
    double maxDy = (fieldSize - 3) - spawnY;
    double maxRadiusFromPlayer = std::sqrt(maxDx * maxDx + maxDy * maxDy);
    
    int numTowers = (fieldSize <= 15) ? 1 : 2;
    
    if (minDistance > maxRadiusFromPlayer) {
        return;
    }
    
    if (fieldSize <= 12) {
        minDistance = std::min(minDistance, fieldSize / 2.5);
    }
    
    for (int i = 0; i < numTowers; ++i) {
        Position towerPos;
        int attempts = 0;
        bool validPosition = false;
        
        int maxAttempts = (fieldSize <= 12) ? 500 : 1000;
        
        while (!validPosition && attempts < maxAttempts) {
            attempts++;
            
            int x, y;
            
            {
                std::uniform_real_distribution<double> angleDist(0.0, 2.0 * M_PI);
                std::uniform_real_distribution<double> radiusDist(minDistance, maxRadiusFromPlayer);
                
                double angle = angleDist(rng);
                double radius = radiusDist(rng);
                
                x = static_cast<int>(spawnX + radius * std::cos(angle));
                y = static_cast<int>(spawnY + radius * std::sin(angle));
                
                if (x < 2 || x >= fieldSize - 2 || y < 2 || y >= fieldSize - 2) {
                    continue;
                }
                
                double actualDx = x - spawnX;
                double actualDy = y - spawnY;
                double actualDistance = std::sqrt(actualDx * actualDx + actualDy * actualDy);
                
                if (actualDistance < minDistance) {
                    continue;
                }
            }
            
            towerPos = Position(x, y);
            
            if (towerPos == playerPosition || buildingManager.hasBuildingAt(towerPos)) {
                continue;
            }
            
            validPosition = true;
        }
        
        if (validPosition) {
            towerManager.addTower(towerPos, std::make_unique<EnemyTower>());
        }
    }
}

bool GameWorld::hasTrapAt(const Position& pos) const {
    return trapManager.hasTrapAt(pos);
}

bool GameWorld::hasTowerAt(const Position& pos) const {
    return towerManager.hasTowerAt(pos);
}

bool GameWorld::isPositionOccupiedByAlly(const Position& pos) const {
    return allyManager.isPositionOccupiedByAlly(pos);
}

TrapManager& GameWorld::getTrapManager() {
    return trapManager;
}

AllyManager& GameWorld::getAllyManager() {
    return allyManager;
}

TowerManager& GameWorld::getTowerManager() {
    return towerManager;
}

EnemyManager& GameWorld::getEnemyManager() {
    return enemyManager;
}

const MapGrid& GameWorld::getMapGrid() const {
    return grid;
}

BuildingManager& GameWorld::getBuildingManager() {
    return buildingManager;
}

int GameWorld::attackTargetAt(const Position& pos, int damage) {
    if (hasAliveEnemyAt(pos)) {
        return enemyManager.attackEnemyAt(pos, damage);
    } else if (hasBuildingAt(pos)) {
        return buildingManager.attackBuildingAt(pos, damage);
    } else if (hasTowerAt(pos)) {
        return towerManager.attackTowerAt(pos, damage);
    }
    return 0;
}

MapGrid& GameWorld::getGrid() {
    return grid;
}

EnemyManager& GameWorld::getEnemyManagerNonConst() {
    return enemyManager;
}

BuildingManager& GameWorld::getBuildingManagerNonConst() {
    return buildingManager;
}

TowerManager& GameWorld::getTowerManagerNonConst() {
    return towerManager;
}

AllyManager& GameWorld::getAllyManagerNonConst() {
    return allyManager;
}

TrapManager& GameWorld::getTrapManagerNonConst() {
    return trapManager;
}

void GameWorld::setPlayerPosition(const Position& pos) {
    playerPosition = pos;
}
