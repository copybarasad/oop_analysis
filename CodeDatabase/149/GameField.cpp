#include "GameField copy.h"
#include "Player.h"
#include "EnemyTower.h"
#include "SaveManager.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>

GameField::GameField(int fieldWidth, int fieldHeight)
    : width(fieldWidth), height(fieldHeight), 
      playerPosition(fieldWidth / 2, fieldHeight / 2),
      towersCanAttackFlag(true) {
    initializeField();
}

GameField::GameField(const GameField& other) {
    copyFrom(other);
}

GameField::GameField(GameField&& other) {
    moveFrom(std::move(other));
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        copyFrom(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) {
    if (this != &other) {
        moveFrom(std::move(other));
    }
    return *this;
}

void GameField::copyFrom(const GameField& other) {
    width = other.width;
    height = other.height;
    grid = other.grid;
    playerPosition = other.playerPosition;
    enemyPositions = other.enemyPositions;
    trapPositions = other.trapPositions;
    trapDamages = other.trapDamages;
    towersCanAttackFlag = other.towersCanAttackFlag;
    
    enemies.clear();
    for (const auto& enemy : other.enemies) {
        enemies.push_back(std::make_unique<Enemy>(*enemy));
    }
}

void GameField::moveFrom(GameField&& other) {
    width = other.width;
    height = other.height;
    grid = std::move(other.grid);
    playerPosition = std::move(other.playerPosition);
    enemyPositions = std::move(other.enemyPositions);
    enemies = std::move(other.enemies);
    trapPositions = std::move(other.trapPositions);
    trapDamages = std::move(other.trapDamages);
    towersCanAttackFlag = other.towersCanAttackFlag;
    
    other.width = 0;
    other.height = 0;
    other.towersCanAttackFlag = true;
}

void GameField::initializeField() {
    grid.resize(height, std::vector<Cell>(width, Cell(EMPTY)));
    
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_int_distribution<> distribution(0, 3);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (distribution(randomGenerator) == 0) {
                grid[y][x] = Cell(WALL);
            }
        }
    }
    
    grid[playerPosition.getY()][playerPosition.getX()] = Cell(EMPTY);
}

void GameField::setPlayerDamage(int damage) {
    playerDamage = damage;
}

bool GameField::movePlayer(int deltaX, int deltaY) {
    Position newPosition = playerPosition;
    newPosition.move(deltaX, deltaY);
    
    if (!isValidPosition(newPosition) || !isCellPassable(newPosition)) {
        return false;
    }
    
    playerPosition = newPosition;
    return true;
}

void GameField::moveEnemies(Player& player) {
    std::vector<Position> directions = {Position(0, 1), Position(1, 0), Position(0, -1), Position(-1, 0)};
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i]->isAlive()) {
            continue;
        }
        
        EnemyTower* tower = dynamic_cast<EnemyTower*>(enemies[i].get());
        if (tower) {
            tower->decreaseCooldown();
            
            Position towerPos = enemyPositions[i];
            int distance = std::abs(playerPosition.getX() - towerPos.getX()) + 
                           std::abs(playerPosition.getY() - towerPos.getY());
            
            if (distance <= tower->getAttackRange() && tower->canAttack()) {
                player.takeDamage(tower->getDamage() / 2);
                tower->setCooldown();
            }
            continue;
        }
        
        Position currentEnemyPos = enemyPositions[i];
        int distanceX = std::abs(currentEnemyPos.getX() - playerPosition.getX());
        int distanceY = std::abs(currentEnemyPos.getY() - playerPosition.getY());
        
        if ((distanceX == 1 && distanceY == 0) || (distanceX == 0 && distanceY == 1)) {
            player.takeDamage(enemies[i]->getDamage());
            continue;
        }
        
        std::shuffle(directions.begin(), directions.end(), randomGenerator);
        
        bool moved = false;
        for (const auto& direction : directions) {
            Position newPosition = enemyPositions[i];
            newPosition.move(direction.getX(), direction.getY());
            
            if (newPosition == playerPosition) {
                player.takeDamage(enemies[i]->getDamage());
                moved = true;
                break;
            }
            
            if (isValidPosition(newPosition) && isCellPassable(newPosition) && 
                !hasEnemyAtPosition(newPosition)) {
                enemyPositions[i] = newPosition;
                moved = true;
                break;
            }
        }
    }
    
    checkTraps();
}

bool GameField::addEnemy(std::unique_ptr<Enemy> enemy, const Position& position) {
    if (!isValidPosition(position)) {
        return false;
    }
    
    setCellType(position, EMPTY);
    
    if (position == playerPosition) {
        return false;
    }
    
    bool isPassableRequired = !enemy->isBuilding();
    if (isPassableRequired && !isCellPassable(position)) {
        return false;
    }
    
    enemies.push_back(std::move(enemy));
    enemyPositions.push_back(position);
    return true;
}

bool GameField::addEnemyFromSave(std::unique_ptr<Enemy> enemy, const Position& pos) {
    enemies.push_back(std::move(enemy));
    enemyPositions.push_back(pos);
    return true;
}

bool GameField::isValidPosition(const Position& pos) const {
    return pos.isValid(width, height);
}

bool GameField::isCellPassable(const Position& pos) const {
    return isValidPosition(pos) && getCell(pos).isPassable();
}

Cell& GameField::getCell(const Position& pos) {
    return grid[pos.getY()][pos.getX()];
}

const Cell& GameField::getCell(const Position& pos) const {
    return grid[pos.getY()][pos.getX()];
}

Position GameField::getPlayerPosition() const {
    return playerPosition;
}

int GameField::getAliveEnemyCount() const {
    int count = 0;
    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            count++;
        }
    }
    return count;
}

bool GameField::hasEnemyAtPosition(const Position& pos) const {
    for (size_t i = 0; i < enemyPositions.size(); ++i) {
        if (enemyPositions[i] == pos && enemies[i]->isAlive()) {
            return true;
        }
    }
    return false;
}

bool GameField::setPlayerPosition(const Position& pos) {
    if (isValidPosition(pos) && isCellPassable(pos)) {
        playerPosition = pos;
        return true;
    }
    return false;
}

void GameField::setCellType(const Position& pos, CellType type) {
    if (isValidPosition(pos)) {
        grid[pos.getY()][pos.getX()] = Cell(type);
    }
}

void GameField::setGridCell(int x, int y, CellType type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = Cell(type);
    }
}

CellType GameField::getCellType(const Position& pos) const {
    if (isValidPosition(pos)) {
        return getCell(pos).getType();
    }
    return WALL;
}

std::vector<Position> GameField::getEnemyPositions() const {
    return enemyPositions;
}

std::vector<int> GameField::getEnemyHealths() const {
    std::vector<int> healths;
    for (const auto& enemy : enemies) {
        healths.push_back(enemy->getHealth());
    }
    return healths;
}

std::vector<int> GameField::getEnemyDamages() const {
    std::vector<int> damages;
    for (const auto& enemy : enemies) {
        damages.push_back(enemy->getDamage());
    }
    return damages;
}

void GameField::damageEnemy(int index, int damage) {
    if (index >= 0 && index < enemies.size()) {
        enemies[index]->takeDamage(damage);
    }
}

bool GameField::placeTrap(const Position& position, int damage) {
    if (!isValidPosition(position) || !isCellPassable(position)) {
        return false;
    }
    
    for (size_t i = 0; i < trapPositions.size(); ++i) {
        if (trapPositions[i] == position) {
            return false;
        }
    }
    
    if (hasEnemyAtPosition(position)) {
        return false;
    }
    
    if (playerPosition == position) {
        return false;
    }
    
    trapPositions.push_back(position);
    trapDamages.push_back(damage);
    return true;
}
void GameField::clearTraps() { 
    trapPositions.clear(); 
    trapDamages.clear(); 
}
void GameField::addTrapFromSave(const Position& pos, int damage) {
    trapPositions.push_back(pos);
    trapDamages.push_back(damage);
}

void GameField::clearEnemies() {
    enemies.clear();
    enemyPositions.clear();
}

void GameField::checkTraps() {
    for (size_t i = 0; i < enemyPositions.size(); ++i) {
        if (!enemies[i]->isAlive()) {
            continue;
        }
        
        for (size_t j = 0; j < trapPositions.size(); ++j) {
            if (enemyPositions[i] == trapPositions[j]) {
                enemies[i]->takeDamage(trapDamages[j]);
                
                trapPositions.erase(trapPositions.begin() + j);
                trapDamages.erase(trapDamages.begin() + j);
                break;
            }
        }
    }
}

bool GameField::isEnemyAlive(int index) const {
    if (index >= 0 && index < enemies.size()) {
        return enemies[index]->isAlive();
    }
    return false;
}

bool GameField::isEnemyBuildingAt(const Position& pos) const {
    for (size_t i = 0; i < enemyPositions.size(); ++i) {
        if (enemyPositions[i] == pos && enemies[i]->isAlive() && enemies[i]->isBuilding()) {
            return true;
        }
    }
    return false;
}

void GameField::damageBuilding(const Position& pos, int damage) {
    for (size_t i = 0; i < enemyPositions.size(); ++i) {
        if (enemyPositions[i] == pos && enemies[i]->isAlive() && enemies[i]->isBuilding()) {
            enemies[i]->takeDamage(damage);
            break;
        }
    }
}

bool GameField::isEnemyTower(int index) const {
    if (index < 0 || index >= enemies.size()) {
        return false;
    }
    EnemyTower* tower = dynamic_cast<EnemyTower*>(enemies[index].get());
    return tower != nullptr;
}

int GameField::getEnemyAttackRange(int index) const {
    if (index < 0 || index >= enemies.size()) {
        return 0;
    }
    EnemyTower* tower = dynamic_cast<EnemyTower*>(enemies[index].get());
    return tower ? tower->getAttackRange() : 0;
}

void GameField::resetTowerAttackStates() {
    for (size_t i = 0; i < enemies.size(); ++i) {
        EnemyTower* tower = dynamic_cast<EnemyTower*>(enemies[i].get());
        if (tower) {
            tower->resetCooldown();
        }
    }
}

bool GameField::canTowerAttack(int towerIndex) const {
    if (towerIndex >= 0 && towerIndex < enemies.size()) {
        EnemyTower* tower = dynamic_cast<EnemyTower*>(enemies[towerIndex].get());
        return tower ? tower->canAttack() : false;
    }
    return false;
}



void GameField::saveFieldState(std::ofstream& file) const {
    SaveManager saveManager;
    saveManager.saveFieldState(file, this);
}

void GameField::loadFieldState(std::ifstream& file) {
    SaveManager saveManager;
    saveManager.loadFieldState(file, this);
}