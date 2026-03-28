#include "BuildingManager.h"
#include "Wanderer.h"
#include <random>
#include <iostream>

BuildingManager::BuildingManager(GameField& field, BeastManager& beastManager, int initialLevel)
    : field(field), beastManager(beastManager), currentLevel(initialLevel) {}

void BuildingManager::updateBuildings() {
    for (auto it = buildings.begin(); it != buildings.end(); ) {
        if (it->second.isDestroyed()) {
            field.getCell(it->first.x, it->first.y).setHasBuilding(false);
            it = buildings.erase(it);
            std::cout << "Beast building destroyed and removed from map!\n";
        } else {
            it->second.update();
            if (it->second.shouldSpawnEnemy()) {
                Position buildingPos = it->first;
                
                std::vector<Position> adjacentPositions = {
                    Position(buildingPos.x + 1, buildingPos.y),
                    Position(buildingPos.x - 1, buildingPos.y),
                    Position(buildingPos.x, buildingPos.y + 1),
                    Position(buildingPos.x, buildingPos.y - 1)
                };
                
                bool foundEmptySpot = false;
                for (const auto& pos : adjacentPositions) {
                    if (field.isCellPassable(pos.x, pos.y) && field.getCell(pos.x, pos.y).isEmpty()) {
                        foundEmptySpot = true;
                        break;
                    }
                }
                
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> spawnChance(0, 3);
                
                if (foundEmptySpot && spawnChance(gen) == 0) {
                    for (const auto& pos : adjacentPositions) {
                        if (field.isCellPassable(pos.x, pos.y) && field.getCell(pos.x, pos.y).isEmpty()) {
                            int beastHealth = 20 + (currentLevel * 5);
                            int beastDamage = 8 + (currentLevel * 2);
                            Beast beast(beastHealth, beastDamage);
                            beastManager.addBeast(pos, beast);
                            std::cout << "Beast building spawned a new enemy!\n";
                            break;
                        }
                    }
                }
            }
            ++it;
        }
    }
}

void BuildingManager::updateTowers(const Position& playerPos, Wanderer& player) {
    for (auto it = towers.begin(); it != towers.end(); ) {
        if (it->second.isDestroyed()) {
            field.getCell(it->first.x, it->first.y).setHasBuilding(false);
            it = towers.erase(it);
            std::cout << "Enemy tower destroyed!\n";
        } else {
            it->second.update();
            if (it->second.canAttack()) {
                it->second.performAttack(it->first, playerPos, field, player);
            }
            ++it;
        }
    }
}

void BuildingManager::spawnBuildings() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> intervalDis(8, 12);
    
    int buildingHealth = 35 + (currentLevel * 10);
    std::uniform_int_distribution<> countDis(1, 2);
    int buildingCount = countDis(gen);
    
    for (int i = 0; i < buildingCount; ++i) {
        int x, y;
        if (field.findEmptyPosition(x, y)) {
            BeastBuilding building(intervalDis(gen), buildingHealth);
            addBuilding(Position(x, y), building);
        }
    }
}

void BuildingManager::spawnTowers() {
    std::random_device rd;
    std::mt19937 gen(rd());
    int towerHealth = 40 + (currentLevel * 15);
    int towerDamage = 15 + (currentLevel * 3);
    std::uniform_int_distribution<> countDis(1, 2);
    int towerCount = countDis(gen);
    
    for (int i = 0; i < towerCount; ++i) {
        int x, y;
        if (field.findEmptyPosition(x, y)) {
            EnemyTower tower(4, towerDamage, 3, towerHealth);
            addTower(Position(x, y), tower);
        }
    }
}

void BuildingManager::addBuilding(const Position& pos, const BeastBuilding& building) {
    buildings.emplace_back(pos, building);
    field.getCell(pos.x, pos.y).setHasBuilding(true);
}

void BuildingManager::addTower(const Position& pos, EnemyTower tower) {
    towers.emplace_back(pos, tower);
    field.getCell(pos.x, pos.y).setHasBuilding(true);
}

const std::vector<std::pair<Position, BeastBuilding>>& BuildingManager::getBuildings() const {
    return buildings;
}

const std::vector<std::pair<Position, EnemyTower>>& BuildingManager::getTowers() const {
    return towers;
}

std::vector<std::pair<Position, BeastBuilding>>& BuildingManager::getBuildings() {
    return buildings;
}

std::vector<std::pair<Position, EnemyTower>>& BuildingManager::getTowers() {
    return towers;
}

void BuildingManager::damageBuilding(const Position& pos, int damage) {
    for (auto& buildingData : buildings) {
        if (buildingData.first.x == pos.x && buildingData.first.y == pos.y) {
            buildingData.second.takeDamage(damage);
            std::cout << "Building takes " << damage << " damage! Health: " 
                      << buildingData.second.getHealth() << "/" << buildingData.second.getMaxHealth() << "\n";
            return;
        }
    }

    for (auto& towerData : towers) {
        if (towerData.first.x == pos.x && towerData.first.y == pos.y) {
            towerData.second.takeDamage(damage);
            std::cout << "Tower takes " << damage << " damage! Health: " 
                      << towerData.second.getHealth() << "/" << towerData.second.getMaxHealth() << "\n";
            return;
        }
    }
}

bool BuildingManager::hasBuildingAt(const Position& pos) const {
    for (const auto& buildingData : buildings) {
        if (buildingData.first.x == pos.x && buildingData.first.y == pos.y) {
            return true;
        }
    }
    
    for (const auto& towerData : towers) {
        if (towerData.first.x == pos.x && towerData.first.y == pos.y) {
            return true;
        }
    }
    
    return false;
}

BuildingType BuildingManager::getBuildingTypeAt(const Position& pos) const {
    for (const auto& buildingData : buildings) {
        if (buildingData.first.x == pos.x && buildingData.first.y == pos.y) {
            return BuildingType::BEAST_BUILDING;
        }
    }
    
    for (const auto& towerData : towers) {
        if (towerData.first.x == pos.x && towerData.first.y == pos.y) {
            return BuildingType::ENEMY_TOWER;
        }
    }
    
    return BuildingType::NONE;
}