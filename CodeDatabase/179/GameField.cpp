#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "Trap.h"
#include "GameObject.h"
#include "GameException.h" 
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>


GameField::GameField(int width, int height)
    : width(std::max(10, std::min(25, width))),
      height(std::max(10, std::min(25, height))) {
    initializeField();
}

GameField::GameField(const GameField& other) {
    deepCopy(other);
}

GameField::GameField(GameField&& other) noexcept
    : width(other.width), height(other.height),
      playerPosition(other.playerPosition),
      player(std::move(other.player)),
      enemies(std::move(other.enemies)),
      buildings(std::move(other.buildings)),
      towers(std::move(other.towers)) {
    field = std::move(other.field);
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        deepCopy(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        playerPosition = other.playerPosition;
        player = std::move(other.player);
        enemies = std::move(other.enemies);
        buildings = std::move(other.buildings);
        towers = std::move(other.towers);
        field = std::move(other.field);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameField::deepCopy(const GameField& other) {
    width = other.width;
    height = other.height;
    playerPosition = other.playerPosition;
    
    field = other.field;
    
    if (other.player) {
        player = std::make_shared<Player>(*other.player);
    }
    
    enemies.clear();
    for (const auto& enemyPair : other.enemies) {
        enemies.emplace_back(enemyPair.first,
                           std::make_shared<Enemy>(*enemyPair.second));
    }
    
    buildings.clear();
    for (const auto& buildingPair : other.buildings) {
        buildings.emplace_back(buildingPair.first,
                             std::make_shared<EnemyBuilding>(*buildingPair.second));
    }
    
    towers.clear();
    for (const auto& towerPair : other.towers) {
        towers.emplace_back(towerPair.first,
                          std::make_shared<EnemyTower>(*towerPair.second));
    }
}

void GameField::initializeField() {
    field.resize(height, std::vector<Cell>(width));
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (dis(gen) == 0) {
                field[y][x].setType(CellType::IMPASSABLE);
            } else if (dis(gen) == 1) {
                field[y][x].setType(CellType::SLOWING);
            }
        }
    }
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

bool GameField::isValidPosition(const Position& pos) const {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

Cell& GameField::getCell(const Position& pos) {
    return field[pos.y][pos.x];
}

const Cell& GameField::getCell(const Position& pos) const {
    return field[pos.y][pos.x];
}

bool GameField::placePlayer(const Position& pos, std::shared_ptr<Player> playerObj) {
    if (!isValidPosition(pos) || !getCell(pos).isPassable()) {
        return false;
    }
    
    if (player) {
        getCell(playerPosition).removeObject();
    }
    
    playerPosition = pos;
    player = playerObj;
    getCell(pos).setObject(player);
    return true;
}

bool GameField::placeEnemy(const Position& pos, std::shared_ptr<Enemy> enemy) {
    if (!isValidPosition(pos) || !getCell(pos).isPassable()) {
        return false;
    }
    
    enemies.emplace_back(pos, enemy);
    getCell(pos).setObject(enemy);
    return true;
}

bool GameField::placeBuilding(const Position& pos, std::shared_ptr<EnemyBuilding> building) {
    if (!isValidPosition(pos) || !getCell(pos).isPassable()) {
        return false;
    }
    
    buildings.emplace_back(pos, building);
    getCell(pos).setObject(building);
    return true;
}

bool GameField::placeTower(const Position& pos, std::shared_ptr<EnemyTower> tower) {
    if (!isValidPosition(pos) || !getCell(pos).isPassable()) {
        return false;
    }
    
    towers.emplace_back(pos, tower);
    getCell(pos).setObject(tower);
    return true;
}

bool GameField::movePlayer(const Position& newPos) {
    if (!isValidPosition(newPos)) return false;
    
    auto& newCell = getCell(newPos);
    
    if (!newCell.isPassable()) return false;
    
    bool isSwamp = (newCell.getType() == CellType::SLOWING);
    
    if (newCell.hasObject()) {
        auto obj = newCell.getObject();
        
        if (auto enemy = std::dynamic_pointer_cast<Enemy>(obj)) {
            enemy->takeDamage(player->getDamage());
            std::cout << "Player attacked " << enemy->getName() << " for " << player->getDamage() << " damage!\n";
            
            if (!enemy->isAlive()) {
                std::cout << enemy->getName() << " destroyed!\n";
                player->addScore(10);
                newCell.removeObject();
                removeDeadEnemies();
            }
            
            if (isSwamp) {
                player->setSkipTurn(true);
                std::cout << "⚡ You attacked from swamp! You'll be stuck next turn.\n";
            }
            return true;
        }
        else if (auto building = std::dynamic_pointer_cast<EnemyBuilding>(obj)) {
            building->takeDamage(player->getDamage());
            std::cout << "Player attacked " << building->getName() << " for " << player->getDamage() << " damage!\n";
            
            if (!building->isAlive()) {
                std::cout << building->getName() << " destroyed!\n";
                player->addScore(25);
                newCell.removeObject();
                buildings.erase(std::remove_if(buildings.begin(), buildings.end(),
                    [&](const auto& pair) { return pair.second == building; }), buildings.end());
            }
            
            if (isSwamp) {
                player->setSkipTurn(true);
                std::cout << "⚡ You attacked from swamp! You'll be stuck next turn.\n";
            }
            return true;
        }
        else if (auto tower = std::dynamic_pointer_cast<EnemyTower>(obj)) {
            tower->takeDamage(player->getDamage());
            std::cout << "Player attacked " << tower->getName() << " for " << player->getDamage() << " damage!\n";
            
            if (!tower->isAlive()) {
                std::cout << tower->getName() << " destroyed!\n";
                player->addScore(30);
                newCell.removeObject();
                towers.erase(std::remove_if(towers.begin(), towers.end(),
                    [&](const auto& pair) { return pair.second == tower; }), towers.end());
            }
            
            if (isSwamp) {
                player->setSkipTurn(true);
                std::cout << "⚡ You attacked from swamp! You'll be stuck next turn.\n";
            }
            return true;
        }
    }
    
    getCell(playerPosition).removeObject();
    playerPosition = newPos;
    getCell(playerPosition).setObject(player);
    
    if (isSwamp) {
        int swampDamage = 5;
        player->takeDamage(swampDamage);
        player->setSkipTurn(true);
        std::cout << "🌀 You entered a swamp! Lost " << swampDamage << " health and will be stuck next turn.\n";
    }
    
    return true;
}

void GameField::moveEnemies() {
    for (auto& enemyPair : enemies) {
        Position currentPos = enemyPair.first;
        auto enemy = enemyPair.second;
        
        if (!enemy->isAlive()) continue;
        
        auto& currentCell = getCell(currentPos);
        if (currentCell.getType() == CellType::SLOWING) {
            enemy->takeDamage(2);
            std::cout << "Enemy is stuck in swamp and takes 2 damage!\n";
            continue;
        }
        
        Position newPos = getRandomAdjacentPosition(currentPos);
        
        if (isValidPosition(newPos)) {
            auto& newCell = getCell(newPos);
            
            if (newCell.isPassable()) {
                bool isMovingToSwamp = (newCell.getType() == CellType::SLOWING);
                
                if (newPos == playerPosition) {
                    player->takeDamage(enemy->getDamage());
                    std::cout << "Enemy attacked player for " << enemy->getDamage() << " damage!\n";
                    
                    if (isMovingToSwamp) {
                        enemy->takeDamage(5);
                        std::cout << "Enemy attacked from swamp and took 5 damage!\n";
                    }
                } else if (newCell.hasObject()) {
                    continue;
                } else {
                    getCell(currentPos).removeObject();
                    enemyPair.first = newPos;
                    getCell(newPos).setObject(enemy);
                    
                    if (isMovingToSwamp) {
                        enemy->takeDamage(3);
                        std::cout << "Enemy entered swamp and took 3 damage!\n";
                    }
                }
            }
        }
    }
}

Position GameField::getPlayerPosition() const {
    return playerPosition;
}

std::shared_ptr<Player> GameField::getPlayer() const {
    return player;
}

std::vector<std::pair<Position, std::shared_ptr<Enemy>>> GameField::getEnemies() const {
    return enemies;
}

std::vector<std::pair<Position, std::shared_ptr<EnemyBuilding>>> GameField::getBuildings() const {
    return buildings;
}

std::vector<std::pair<Position, std::shared_ptr<EnemyTower>>> GameField::getTowers() const {
    return towers;
}

void GameField::removeDeadEnemies() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const auto& enemyPair) {
            return !enemyPair.second->isAlive();
        }), enemies.end());
}

std::vector<std::shared_ptr<Enemy>> GameField::getEnemiesToSpawn() {
    std::vector<std::shared_ptr<Enemy>> enemiesToSpawn;
    
    for (auto& buildingPair : buildings) {
        auto building = buildingPair.second;
        building->update();
        
        if (building->shouldSpawnEnemy()) {
            enemiesToSpawn.push_back(std::make_shared<Enemy>());
            building->resetCounter();
        }
    }
    
    return enemiesToSpawn;
}

void GameField::attackPlayerFromTowers() {
    for (auto& towerPair : towers) {
        auto tower = towerPair.second;
        
        tower->updateCooldown();
        
        if (tower->isAlive() && tower->canAttackNow()) {
            Position towerPos = towerPair.first;
            Position playerPos = playerPosition;
            
            int distance = std::abs(towerPos.x - playerPos.x) + std::abs(towerPos.y - playerPos.y);
            
            if (distance <= tower->getAttackRadius()) {
                int weakenedDamage = tower->getWeakenedDamage();
                player->takeDamage(weakenedDamage);
                
                std::cout << " Enemy tower used weakened spell on player for "
                          << weakenedDamage << " damage! (Range: " << distance << ")\n";
                
                tower->setCooldown(1);
            }
        }
    }
}

Position GameField::getRandomAdjacentPosition(const Position& pos) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 3);
    
    int direction = dis(gen);
    Position newPos = pos;
    
    switch (direction) {
        case 0: newPos.x++; break;
        case 1: newPos.x--; break;
        case 2: newPos.y++; break;
        case 3: newPos.y--; break;
    }
    
    return newPos;
}

bool GameField::rangedAttack(const Position& target, std::shared_ptr<Player> attacker) {
    if (!isValidPosition(target)) return false;
    
    // Проверяем расстояние
    Position attackerPos = playerPosition;
    int distance = std::abs(target.x - attackerPos.x) + std::abs(target.y - attackerPos.y);
    
    if (distance > attacker->getRangedAttackRadius()) {
        std::cout << "Target is too far! Maximum range: " << attacker->getRangedAttackRadius() << "\n";
        return false;
    }
    
    if (distance == 0) {
        std::cout << "Cannot attack yourself!\n";
        return false;
    }
    
    auto& targetCell = getCell(target);
    if (!targetCell.hasObject()) {
        std::cout << "No target at specified position!\n";
        return false;
    }
    
    auto obj = targetCell.getObject();
    
    // Проверяем что цель - враг
    if (auto enemy = std::dynamic_pointer_cast<Enemy>(obj)) {
        enemy->takeDamage(attacker->getRangedDamage());
        std::cout << "Ranged attack hit " << enemy->getName() << " for " << attacker->getRangedDamage() << " damage!\n";
        
        if (!enemy->isAlive()) {
            std::cout << enemy->getName() << " destroyed!\n";
            attacker->addScore(10);
            targetCell.removeObject();
            removeDeadEnemies();
        }
        return true;
    }
    else if (auto building = std::dynamic_pointer_cast<EnemyBuilding>(obj)) {
        building->takeDamage(attacker->getRangedDamage());
        std::cout << "Ranged attack hit " << building->getName() << " for " << attacker->getRangedDamage() << " damage!\n";
        
        if (!building->isAlive()) {
            std::cout << building->getName() << " destroyed!\n";
            attacker->addScore(25);
            targetCell.removeObject();
            buildings.erase(std::remove_if(buildings.begin(), buildings.end(),
                [&](const auto& pair) { return pair.second == building; }), buildings.end());
        }
        return true;
    }
    else if (auto tower = std::dynamic_pointer_cast<EnemyTower>(obj)) {
        tower->takeDamage(attacker->getRangedDamage());
        std::cout << "Ranged attack hit " << tower->getName() << " for " << attacker->getRangedDamage() << " damage!\n";
        
        if (!tower->isAlive()) {
            std::cout << tower->getName() << " destroyed!\n";
            attacker->addScore(30);
            targetCell.removeObject();
            towers.erase(std::remove_if(towers.begin(), towers.end(),
                [&](const auto& pair) { return pair.second == tower; }), towers.end());
        }
        return true;
    }
    
    std::cout << "Invalid target for ranged attack!\n";
    return false;
}

bool GameField::placeAlly(const Position& pos, std::shared_ptr<Ally> ally) {
    if (!isValidPosition(pos) || !getCell(pos).isPassable()) {
        return false;
    }
    
    allies.emplace_back(pos, ally);
    getCell(pos).setObject(ally);
    return true;
}

void GameField::moveAllies() {
    for (auto& allyPair : allies) {
        Position currentPos = allyPair.first;
        auto ally = allyPair.second;
        
        if (!ally->isAlive()) continue;
        
        Position newPos = getRandomAdjacentPosition(currentPos);
        
        if (isValidPosition(newPos)) {
            auto& newCell = getCell(newPos);
            
            if (newCell.isPassable()) {
                if (newCell.hasObject()) {
                    auto obj = newCell.getObject();
                    if (auto enemy = std::dynamic_pointer_cast<Enemy>(obj)) {
                        enemy->takeDamage(ally->getDamage());
                        std::cout << "Ally attacked enemy for " << ally->getDamage() << " damage!\n";
                        
                        if (!enemy->isAlive()) {
                            std::cout << "Enemy destroyed by ally!\n";
                            newCell.removeObject();
                            removeDeadEnemies();
                        }
                    }
                } else {
                    getCell(currentPos).removeObject();
                    allyPair.first = newPos;
                    getCell(newPos).setObject(ally);
                }
            }
        }
    }
}

void GameField::checkTraps() {
    for (auto& enemyPair : enemies) {
        Position enemyPos = enemyPair.first;
        auto& cell = getCell(enemyPos);
        
        if (cell.hasObject()) {
            auto obj = cell.getObject();
            if (auto trap = std::dynamic_pointer_cast<Trap>(obj)) {
                if (trap->isActive()) {
                    auto enemy = enemyPair.second;
                    enemy->takeDamage(trap->getDamage());
                    trap->trigger();
                    std::cout << "💥 Trap triggered! Enemy took " << trap->getDamage() << " damage!\n";
                    
                    if (!enemy->isAlive()) {
                        std::cout << "Enemy destroyed by trap!\n";
                        cell.removeObject();
                        removeDeadEnemies();
                    }
                    
                    if (!trap->isAlive()) {
                        cell.removeObject();
                    }
                }
            }
        }
    }
}

std::vector<std::pair<Position, std::shared_ptr<Ally>>> GameField::getAllies() const {
    return allies;
}

void GameField::clearField() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            field[y][x].removeObject();
        }
    }
    
    enemies.clear();
    buildings.clear();
    towers.clear();
    allies.clear();
    
    player = nullptr;
    playerPosition = Position(0, 0);
}

void GameField::save(std::ostream& file) const {
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    
    file.write(reinterpret_cast<const char*>(&playerPosition.x), sizeof(playerPosition.x));
    file.write(reinterpret_cast<const char*>(&playerPosition.y), sizeof(playerPosition.y));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            CellType type = field[y][x].getType();
            file.write(reinterpret_cast<const char*>(&type), sizeof(type));
        }
    }
    
    size_t enemyCount = enemies.size();
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    for (const auto& enemyPair : enemies) {
        file.write(reinterpret_cast<const char*>(&enemyPair.first.x), sizeof(enemyPair.first.x));
        file.write(reinterpret_cast<const char*>(&enemyPair.first.y), sizeof(enemyPair.first.y));
        int health = enemyPair.second->getHealth();
        int damage = enemyPair.second->getDamage();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    }
    
    size_t buildingCount = buildings.size();
    file.write(reinterpret_cast<const char*>(&buildingCount), sizeof(buildingCount));
    for (const auto& buildingPair : buildings) {
        file.write(reinterpret_cast<const char*>(&buildingPair.first.x), sizeof(buildingPair.first.x));
        file.write(reinterpret_cast<const char*>(&buildingPair.first.y), sizeof(buildingPair.first.y));
        int health = buildingPair.second->getHealth();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
    }
    
    size_t towerCount = towers.size();
    file.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
    for (const auto& towerPair : towers) {
        file.write(reinterpret_cast<const char*>(&towerPair.first.x), sizeof(towerPair.first.x));
        file.write(reinterpret_cast<const char*>(&towerPair.first.y), sizeof(towerPair.first.y));
        int health = towerPair.second->getHealth();
        int damage = towerPair.second->getDamage();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    }
    
    size_t allyCount = allies.size();
    file.write(reinterpret_cast<const char*>(&allyCount), sizeof(allyCount));
    for (const auto& allyPair : allies) {
        file.write(reinterpret_cast<const char*>(&allyPair.first.x), sizeof(allyPair.first.x));
        file.write(reinterpret_cast<const char*>(&allyPair.first.y), sizeof(allyPair.first.y));
        int health = allyPair.second->getHealth();
        int damage = allyPair.second->getDamage();
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    }
}

void GameField::load(std::istream& file) {
    // Сначала читаем размеры поля
    int newWidth, newHeight;
    file.read(reinterpret_cast<char*>(&newWidth), sizeof(newWidth));
    file.read(reinterpret_cast<char*>(&newHeight), sizeof(newHeight));
    
    if (file.fail()) {
        throw CorruptedSaveException("Cannot read field dimensions");
    }
    
    // Проверяем корректность размеров
    if (newWidth < 10 || newWidth > 25 || newHeight < 10 || newHeight > 25) {
        throw CorruptedSaveException("Invalid field dimensions");
    }
    
    // Очищаем текущее состояние
    clearField();
    
    // Устанавливаем новые размеры
    width = newWidth;
    height = newHeight;
    
    // Инициализируем поле с правильными размерами
    field.clear();
    field.resize(height);
    for (int y = 0; y < height; ++y) {
        field[y].resize(width);
        for (int x = 0; x < width; ++x) {
            field[y][x] = Cell();
        }
    }
    
    // Загружаем позицию игрока
    file.read(reinterpret_cast<char*>(&playerPosition.x), sizeof(playerPosition.x));
    file.read(reinterpret_cast<char*>(&playerPosition.y), sizeof(playerPosition.y));
    
    if (file.fail()) {
        throw CorruptedSaveException("Cannot read player position");
    }
    
    // Проверяем позицию игрока
    if (playerPosition.x < 0 || playerPosition.x >= width ||
        playerPosition.y < 0 || playerPosition.y >= height) {
        throw CorruptedSaveException("Player position out of bounds");
    }
    
    // Загружаем тип каждой клетки
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            CellType type;
            file.read(reinterpret_cast<char*>(&type), sizeof(type));
            if (file.fail()) {
                throw CorruptedSaveException("Cannot read cell type at (" +
                                           std::to_string(x) + "," + std::to_string(y) + ")");
            }
            field[y][x].setType(type);
        }
    }
    
    // Загружаем врагов
    size_t enemyCount;
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
    for (size_t i = 0; i < enemyCount; ++i) {
        Position pos;
        int health, damage;
        file.read(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
        file.read(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        
        if (file.fail()) {
            throw CorruptedSaveException("Cannot read enemy data");
        }
        
        if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height) {
            auto enemy = std::make_shared<Enemy>(health, damage);
            placeEnemy(pos, enemy);
        }
    }
    
    // Загружаем здания
    size_t buildingCount;
    file.read(reinterpret_cast<char*>(&buildingCount), sizeof(buildingCount));
    for (size_t i = 0; i < buildingCount; ++i) {
        Position pos;
        int health;
        file.read(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
        file.read(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        
        if (file.fail()) {
            throw CorruptedSaveException("Cannot read building data");
        }
        
        if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height) {
            auto building = std::make_shared<EnemyBuilding>(health, 5);
            placeBuilding(pos, building);
        }
    }
    
    // Загружаем башни
    size_t towerCount;
    file.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
    for (size_t i = 0; i < towerCount; ++i) {
        Position pos;
        int health, damage;
        file.read(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
        file.read(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        
        if (file.fail()) {
            throw CorruptedSaveException("Cannot read tower data");
        }
        
        if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height) {
            auto tower = std::make_shared<EnemyTower>(health, damage);
            placeTower(pos, tower);
        }
    }
    
    // Загружаем союзников
    size_t allyCount;
    file.read(reinterpret_cast<char*>(&allyCount), sizeof(allyCount));
    for (size_t i = 0; i < allyCount; ++i) {
        Position pos;
        int health, damage;
        file.read(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
        file.read(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
        
        if (file.fail()) {
            throw CorruptedSaveException("Cannot read ally data");
        }
        
        if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height) {
            auto ally = std::make_shared<Ally>(health, damage);
            placeAlly(pos, ally);
        }
    }
    
    // Размещаем игрока
    if (player) {
        auto& cell = getCell(playerPosition);
        if (cell.hasObject()) {
            cell.removeObject();
        }
        cell.setObject(player);
    }
}
