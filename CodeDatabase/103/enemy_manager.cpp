#include "enemy_manager.h"

EnemyManager::EnemyManager(const std::vector<std::pair<int, int>>& initialPositions) {
    for (const auto& pos : initialPositions) {
        enemies[pos] = new Enemy();
    }
}

EnemyManager::~EnemyManager() {
    for (auto& pair : enemies) {
        delete pair.second;
    }
    enemies.clear();
}

void EnemyManager::attackEnemyAt(int x, int y, int damage) {
    Enemy* enemy = getEnemyAt(x, y);
    if (enemy) {
        enemy->takeDamage(damage);
    }
}

bool EnemyManager::isEnemyAliveAt(int x, int y) const {
    Enemy* enemy = getEnemyAt(x, y);
    return enemy && enemy->isAlive();
}

void EnemyManager::addEnemy(Enemy* enemy, int x, int y) {
    if (enemy != nullptr) {
        auto pos = std::pair{x, y};
        enemies[pos] = enemy;
    }
}


int EnemyManager::getEnemyDamageAt(int x, int y) const {
    Enemy* enemy = getEnemyAt(x, y);
    return enemy ? enemy->getDamage() : 0;
}

Enemy* EnemyManager::getEnemyAt(int x, int y) const {
    auto pos = std::pair(x, y);
    auto it = enemies.find(pos);
    if (it != enemies.end()) {
        return it->second;
    }
    return nullptr;
}

void EnemyManager::moveEnemy(int fromX, int fromY, int toX, int toY) {
    auto fromPos = std::make_pair(fromX, fromY);
    auto toPos = std::make_pair(toX, toY);
    
    auto it = enemies.find(fromPos);
    if (it != enemies.end() && enemies.find(toPos) == enemies.end()) {
        Enemy* enemy = it->second;
        enemies.erase(it);
        enemies[toPos] = enemy;
    }
}

void EnemyManager::removeEnemyAt(int x, int y) {
    auto pos = std::make_pair(x, y);
    auto it = enemies.find(pos);
    if (it != enemies.end()) {
        delete it->second;
        enemies.erase(it);
    }
}

bool EnemyManager::hasEnemyAt(int x, int y) const {
    auto pos = std::make_pair(x, y);
    return enemies.find(pos) != enemies.end();
}


std::vector<std::tuple<int, int, int, int>> EnemyManager::getEnemiesData() const {
    std::vector<std::tuple<int, int, int, int>> data;
    for (const auto& [pos, enemy] : enemies) {
        if (enemy && enemy->isAlive()) {
            data.emplace_back(pos.first, pos.second, enemy->getHealth(), enemy->getDamage());
        }
    }
    return data;
}
std::vector<std::pair<int, int>> EnemyManager::getAliveEnemyPositions() const {
    std::vector<std::pair<int, int>> positions;
    for (const auto& [pos, enemy] : enemies) {
        if (enemy && enemy->isAlive()) {
            positions.push_back(pos);
        }
    }
    return positions;
}
