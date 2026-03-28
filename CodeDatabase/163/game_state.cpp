#include "game_state.hpp"
#include "enemy.hpp"
#include "enemy_building.hpp"
#include "game_exceptions.hpp"
#include <iostream>
#include <algorithm>

GameState::GameState() 
    : currentTurn(1), isGameRunning(true), currentLevel(1), score(0) {
}

void GameState::initializeNewGame() {
    createMapForLevel(currentLevel);
    spawnEntitiesForLevel(currentLevel);
}

void GameState::initializeLevel(int level) {
    currentLevel = level;
    currentTurn = 1;
    isGameRunning = true;
    enemies.clear();
    buildings.clear();
    
    createMapForLevel(level);
    spawnEntitiesForLevel(level);
}

void GameState::cleanupGame() {
    enemies.clear();
    buildings.clear();
    player.reset();
    map.reset();
    isGameRunning = false;
}

Map* GameState::getMap() const {
    return map.get();
}

Player* GameState::getPlayer() const {
    return player.get();
}

const std::vector<std::unique_ptr<GameObject>>& GameState::getEnemies() const {
    return enemies;
}

const std::vector<std::unique_ptr<GameObject>>& GameState::getBuildings() const {
    return buildings;
}

int GameState::getCurrentTurn() const {
    return currentTurn;
}

int GameState::getCurrentLevel() const {
    return currentLevel;
}

int GameState::getScore() const {
    return score;
}

bool GameState::isGameActive() const {
    return isGameRunning && player && player->isAlive();
}

void GameState::endGame() {
    if (isGameRunning) {
        isGameRunning = false;
    }
}

void GameState::addScore(int points) {
    if (points > 0) {
        score += points;
    }
}

void GameState::nextTurn() {
    if (!isGameActive()) return;
    currentTurn++;
}

void GameState::processEnemyTurns() {
    if (!player || !player->isAlive()) return;
    
    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            enemy->takeTurn(*map);
        }
    }
    
    for (auto& building : buildings) {
        if (building && building->isAlive()) {
            building->takeTurn(*map);
        }
    }
    
    cleanupDeadObjects();
}

void GameState::cleanupDeadObjects() {
    int killed = 0;
    for (const auto& enemy : enemies) {
        if (enemy && !enemy->isAlive()) {
            killed++;
        }
    }
    
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj || !obj->isAlive();
            }),
        enemies.end()
    );
    
    buildings.erase(
        std::remove_if(buildings.begin(), buildings.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj || !obj->isAlive();
            }),
        buildings.end()
    );
    
    for (int i = 0; i < killed; i++) {
        enemyDefeated();
    }
}

void GameState::enemyDefeated() {
    if (player) {
        player->enemyDefeated();
    }
    addScore(10);
}

bool GameState::isVictory() const {
    if (!player || !player->isAlive()) {
        return false;
    }
    
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            return false;
        }
    }
    
    for (const auto& building : buildings) {
        if (building && building->isAlive()) {
            return false;
        }
    }
    
    return true;
}

bool GameState::isDefeat() const {
    return !player || !player->isAlive();
}

void GameState::createMapForLevel(int level) {
    map = std::make_unique<Map>(10, 10);
    
    if (level == 1) {
        for (int i = 3; i < 7; i++) {
            map->setCellType(i, 3, Cell::Type::BLOCKING);
        }
        
        map->setCellType(4, 5, Cell::Type::SLOWING);
        map->setCellType(5, 5, Cell::Type::SLOWING);
    }
}

void GameState::spawnEntitiesForLevel(int level) {
    if (!map) {
        throw std::runtime_error("Map not created for spawnEntitiesForLevel");
    }
    
    try {
        int startX = 2;
        int startY = 2;
        
        Cell& cell = map->getCell(startX, startY);
        player = std::make_unique<Player>(
            startX, startY, cell, 300, 2, 200, 150, 5);
        
        if (level == 1) {
            for (int i = 0; i < 3; i++) {
                int enemyX = 5 + i;
                int enemyY = 5;
                
                if (isValidSpawnPosition(enemyX, enemyY)) {
                    Cell& enemyCell = map->getCell(enemyX, enemyY);
                    auto enemy = std::make_unique<Enemy>(
                        enemyX, enemyY, enemyCell, 50, 2, 10, player.get());
                    enemies.push_back(std::move(enemy));
                }
            }
            
            int buildingX = 7;
            int buildingY = 7;
            
            if (isValidSpawnPosition(buildingX, buildingY)) {
                Cell& buildingCell = map->getCell(buildingX, buildingY);
                auto building = std::make_unique<EnemyBuilding>(
                    buildingX, buildingY, buildingCell, 100, 3, enemies, player.get());
                buildings.push_back(std::move(building));
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error spawning entities: " << e.what() << "\n";
    }
}

bool GameState::isValidSpawnPosition(int x, int y) const {
    if (!map) return false;
    if (!map->isValidPosition(x, y)) return false;
    
    const Cell& cell = map->getCell(x, y);
    return cell.isPassable() && cell.isEmptyCell();
}

void GameState::saveToFile(const std::string& filename) const {
    std::cout << "Сохранение в файл " << filename << "\n";
    (void)filename;
}

bool GameState::loadFromFile(const std::string& filename) {
    std::cout << "Загрузка из файл " << filename << "\n";
    (void)filename;
    return true;
}

void GameState::setCurrentTurn(int turn) {
    currentTurn = turn;
}

void GameState::setCurrentLevel(int level) {
    currentLevel = level;
}

void GameState::setScore(int newScore) {
    score = newScore;
}

void GameState::createPlayer(int x, int y, Cell& cell, int hp, int maxHp) {
    player = std::make_unique<Player>(x, y, cell, hp, 2, 20, 15, 5);
    (void)maxHp;
}

void GameState::addEnemy(std::unique_ptr<GameObject> enemy) {
    enemies.push_back(std::move(enemy));
}

void GameState::addBuilding(std::unique_ptr<GameObject> building) {
    buildings.push_back(std::move(building));
}

GameState::SerializedData GameState::serialize() const {
    SerializedData data;
    data.currentTurn = currentTurn;
    data.currentLevel = currentLevel;
    data.score = score;
    return data;
}

void GameState::deserialize(const SerializedData& data) {
    currentTurn = data.currentTurn;
    currentLevel = data.currentLevel;
    score = data.score;
    isGameRunning = true;
}