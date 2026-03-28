#include "../include/TowerManager.h"
#include "../include/Player.h"
#include <cmath>

void TowerManager::addTower(const Position& pos, std::unique_ptr<EnemyTower> tower) {
    towers.push_back({pos, std::move(tower)});
}

bool TowerManager::hasTowerAt(const Position& pos) const {
    for (const auto& towerPair : towers) {
        if (towerPair.first == pos) {
            return true;
        }
    }
    return false;
}

void TowerManager::processTowers(Player& player, const Position& playerPosition) {
    for (auto& towerPair : towers) {
        towerPair.second->processTurn();
        
        Position towerPos = towerPair.first;
        int dx = playerPosition.getX() - towerPos.getX();
        int dy = playerPosition.getY() - towerPos.getY();
        int distance = abs(dx) + abs(dy);
        
        if (distance <= towerPair.second->getRange() && towerPair.second->canAttack()) {
            player.takeDamage(towerPair.second->getDamage());
            towerPair.second->attack();
        }
    }
}

int TowerManager::attackTowerAt(const Position& pos, int damage) {
    for (auto& towerPair : towers) {
        if (towerPair.first == pos && towerPair.second->isAlive()) {
            return towerPair.second->takeDamage(damage);
        }
    }
    return 0;
}

int TowerManager::removeDeadTowers() {
    int destroyedCount = 0;
    for (auto it = towers.begin(); it != towers.end(); ) {
        if (!it->second->isAlive()) {
            it = towers.erase(it);
            destroyedCount++;
        } else {
            ++it;
        }
    }
    return destroyedCount;
}

const std::vector<std::pair<Position, std::unique_ptr<EnemyTower>>>& TowerManager::getTowers() const {
    return towers;
}
