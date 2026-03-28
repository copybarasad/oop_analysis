#include "../include/EnemyManager.h"

void EnemyManager::addEnemy(const Position& pos, std::unique_ptr<Enemy> enemy) {
    enemies.push_back({pos, std::move(enemy)});
}

int EnemyManager::attackEnemyAt(const Position& pos, int damage) {
    int index = getEnemyIndexAt(pos);
    if (index != -1) {
        return enemies[index].second->takeDamage(damage);
    }
    return 0;
}

int EnemyManager::removeDeadEnemies() {
    int killedCount = 0;
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!it->second->isAlive()) {
            it = enemies.erase(it);
            killedCount++;
        } else {
            ++it;
        }
    }
    return killedCount;
}

bool EnemyManager::hasAliveEnemyAt(const Position& pos) const {
    for (const auto& enemyPair : enemies) {
        if (enemyPair.first == pos && enemyPair.second->isAlive()) {
            return true;
        }
    }
    return false;
}

bool EnemyManager::isPositionOccupiedByEnemy(const Position& pos) const {
    for (const auto& enemyPair : enemies) {
        if (enemyPair.first == pos && enemyPair.second->isAlive()) {
            return true;
        }
    }
    return false;
}

const std::vector<std::pair<Position, std::unique_ptr<Enemy>>>& EnemyManager::getEnemies() const {
    return enemies;
}

std::vector<std::pair<Position, std::unique_ptr<Enemy>>>& EnemyManager::getEnemies() {
    return enemies;
}

int EnemyManager::getEnemyIndexAt(const Position& pos) const {
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i].first == pos && enemies[i].second->isAlive()) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
