#ifndef BUILDINGMANAGER_H
#define BUILDINGMANAGER_H

#include "GameField.h"
#include "BeastBuilding.h"
#include "BeastManager.h"
#include "EnemyTower.h"
#include "Wanderer.h"
#include <vector>
#include <memory>

enum class BuildingType {
    NONE,
    BEAST_BUILDING,
    ENEMY_TOWER
};

class BuildingManager {
private:
    GameField& field;
    BeastManager& beastManager;
    std::vector<std::pair<Position, BeastBuilding>> buildings;
    std::vector<std::pair<Position, EnemyTower>> towers;
    int currentLevel;

public:
    BuildingManager(GameField& field, BeastManager& beastManager, int initialLevel = 1);
    
    void updateBuildings();
    void updateTowers(const Position& playerPos, Wanderer& player);
    void spawnBuildings();
    void spawnTowers();
    void addBuilding(const Position& pos, const BeastBuilding& building);
    void addTower(const Position& pos, EnemyTower tower);
    
    const std::vector<std::pair<Position, BeastBuilding>>& getBuildings() const;
    const std::vector<std::pair<Position, EnemyTower>>& getTowers() const;
    
    std::vector<std::pair<Position, BeastBuilding>>& getBuildings();
    std::vector<std::pair<Position, EnemyTower>>& getTowers();
    
    void damageBuilding(const Position& pos, int damage);
    bool hasBuildingAt(const Position& pos) const;
    BuildingType getBuildingTypeAt(const Position& pos) const;
    
    void setCurrentLevel(int level) { currentLevel = level; }
    int getCurrentLevel() const { return currentLevel; }
};

#endif