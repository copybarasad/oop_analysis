#include "tower_manager.h"

TowerManager::~TowerManager() {
    for (auto& [pos, tower] : towers) {
        delete tower;
    }
}

void TowerManager::addTower(Tower* tower) {
    towers[tower->getPosition()] = tower;
}

bool TowerManager::hasTowerAt(int x, int y) const {
    auto it = towers.find({x, y});
    return it != towers.end() && it->second->isAlive();
}

void TowerManager::damageTowerAt(int x, int y, int dmg) {
    auto it = towers.find({x, y});
    if (it != towers.end() && it->second->isAlive()) {
        it->second->takeDamage(dmg);
        if (!it->second->isAlive()) {
            delete it->second;        
            towers.erase(it);          
        }
    }
}

void TowerManager::updateTowersCooldown() {
    for (auto& towerEntry : towers) {
        if (towerEntry.second) {
            towerEntry.second->updateCooldown();
        }
    }
}

std::vector<std::tuple<int, int, int, int, int>> TowerManager::getTowersData() const {
    std::vector<std::tuple<int, int, int, int, int>> data;
    for (const auto& [pos, tower] : towers) {
        if (tower && tower->isAlive()) {
            data.emplace_back(pos.first, pos.second, tower->getHealth(), tower->getDamage(), tower->getRadius());
        }
    }
    return data;
}

const std::map<std::pair<int, int>, Tower*>& TowerManager::getAllTowers() const {
    return towers;
}