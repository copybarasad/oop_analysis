#include "EnemyBuilding.h"
#include "GameField.h"
#include "Enemy.h"
#include <stdexcept>
#include <random>
#include <sstream>
#include <string>

EnemyBuilding::EnemyBuilding(int startX, int startY, int spawnTurns, int buildingId)
    : positionX(startX), positionY(startY), spawnCooldown(spawnTurns), 
      currentCooldown(spawnTurns), buildingId(buildingId), enemyCounter(0),
      health(100), maxHealth(100) {
    
    if (spawnTurns <= 0) {
        throw std::invalid_argument("Spawn cooldown must be positive");
    }
    
    validateInvariant();
}

void EnemyBuilding::validateInvariant() const {
    if (spawnCooldown <= 0 || currentCooldown < 0 || 
        currentCooldown > spawnCooldown) {
        throw std::runtime_error("EnemyBuilding invariant violated");
    }
}

int EnemyBuilding::getPositionX() const {
    return positionX;
}

int EnemyBuilding::getPositionY() const {
    return positionY;
}

int EnemyBuilding::getSpawnCooldown() const {
    return spawnCooldown;
}

int EnemyBuilding::getCurrentCooldown() const {
    return currentCooldown;
}

int EnemyBuilding::getId() const {
    return buildingId;
}

bool EnemyBuilding::canSpawn() const {
    return currentCooldown <= 0;
}

void EnemyBuilding::updateCooldown() {
    if (currentCooldown > 0) {
        currentCooldown--;
    }
}

void EnemyBuilding::resetCooldown() {
    currentCooldown = spawnCooldown;
}

bool EnemyBuilding::findSpawnPosition(const GameField& field, int& spawnX, int& spawnY) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Try to find empty adjacent cells
    std::vector<std::pair<int, int>> adjacentPositions;
    
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // Skip building position
            
            int checkX = positionX + dx;
            int checkY = positionY + dy;
            
            if (field.canMoveTo(checkX, checkY)) {
                adjacentPositions.push_back({checkX, checkY});
            }
        }
    }
    
    if (adjacentPositions.empty()) {
        return false;
    }
    
    // Randomly select one of the available positions
    std::uniform_int_distribution<> dis(0, adjacentPositions.size() - 1);
    int selectedIndex = dis(gen);
    
    spawnX = adjacentPositions[selectedIndex].first;
    spawnY = adjacentPositions[selectedIndex].second;
    
    return true;
}

bool EnemyBuilding::trySpawnEnemy(GameField& field, std::vector<Enemy>& enemies) {
    // Используем базовые параметры по умолчанию
    return trySpawnEnemy(field, enemies, 50, 10);
}

bool EnemyBuilding::trySpawnEnemy(GameField& field, std::vector<Enemy>& enemies, int enemyHealth, int enemyDamage) {
    if (!canSpawn()) {
        return false;
    }
    
    int spawnX, spawnY;
    if (!findSpawnPosition(field, spawnX, spawnY)) {
        return false; // No available spawn position
    }
    
    // Create new enemy with specified parameters
    int newEnemyId = enemyCounter++;
    Enemy newEnemy(spawnX, spawnY, enemyHealth, enemyDamage, newEnemyId);
    
    // Add to enemies vector
    enemies.push_back(newEnemy);
    
    // Set position on field
    field.setEnemyPosition(spawnX, spawnY);
    
    // Reset cooldown
    resetCooldown();
    
    return true;
}

void EnemyBuilding::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool EnemyBuilding::isDestroyed() const {
    return health <= 0;
}

std::string EnemyBuilding::serialize() const {
    std::ostringstream oss;
    // Формат: positionX:positionY:spawnCooldown:currentCooldown:buildingId:enemyCounter:health:maxHealth
    oss << positionX << ":" << positionY << ":" << spawnCooldown << ":"
        << currentCooldown << ":" << buildingId << ":" << enemyCounter << ":"
        << health << ":" << maxHealth << "\n";
    return oss.str();
}

bool EnemyBuilding::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    
    // positionX
    if (!std::getline(iss, token, ':')) return false;
    positionX = std::stoi(token);
    
    // positionY
    if (!std::getline(iss, token, ':')) return false;
    positionY = std::stoi(token);
    
    // spawnCooldown
    if (!std::getline(iss, token, ':')) return false;
    spawnCooldown = std::stoi(token);
    
    // currentCooldown
    if (!std::getline(iss, token, ':')) return false;
    currentCooldown = std::stoi(token);
    
    // buildingId
    if (!std::getline(iss, token, ':')) return false;
    buildingId = std::stoi(token);
    
    // enemyCounter
    if (!std::getline(iss, token, ':')) return false;
    enemyCounter = std::stoi(token);
    
    // health
    if (!std::getline(iss, token, ':')) return false;
    health = std::stoi(token);
    
    // maxHealth
    if (!std::getline(iss, token)) return false;
    if (!token.empty() && token.back() == '\n') {
        token.pop_back();
    }
    if (!token.empty() && token.back() == '\r') {
        token.pop_back();
    }
    maxHealth = std::stoi(token);
    
    validateInvariant();
    return true;
}

