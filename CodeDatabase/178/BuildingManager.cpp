#include "BuildingManager.h"
#include <iostream>

BuildingManager::BuildingManager(std::vector<EnemyBuilding>& enemyBuildings, std::vector<EnemyTower>& enemyTowers) 
    : buildings(enemyBuildings), towers(enemyTowers) {}

void BuildingManager::updateBuildings(std::vector<Enemy> &enemies, GameField &field) {
    for (auto &building : buildings) {
        building.update();
        if (building.shouldSpawnEnemy()) {
            Enemy newEnemy = building.spawnEnemy();
            if (field.isValidPosition(newEnemy.getPosition()) && field.isPositionPassable(newEnemy.getPosition())) {
                enemies.push_back(newEnemy);
                std::cout << "New enemy spawned from building!\n";
            }
        }
    }
}

EnemyBuilding* BuildingManager::getBuildingAtPosition(const Position &pos) {
    for (auto &b : buildings) {
        if (b.getPosition() == pos) return &b;
    }
    return nullptr;
}

EnemyTower* BuildingManager::getTowerAtPosition(const Position &pos) {
    for (auto &t : towers) {
        if (t.getPosition() == pos && t.isAlive()) return &t;
    }
    return nullptr;
}

void BuildingManager::addTower(const EnemyTower &tower) {
    towers.push_back(tower);
}