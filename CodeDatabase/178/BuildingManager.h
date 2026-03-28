#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "CharacterFactory.h"
#include "GameField.h"
#include <vector>

class BuildingManager {
private:
    std::vector<EnemyBuilding>& buildings;
    std::vector<EnemyTower>& towers;
    
public:
    BuildingManager(std::vector<EnemyBuilding>& enemyBuildings, std::vector<EnemyTower>& enemyTowers);
    void updateBuildings(std::vector<Enemy> &enemies, GameField &field);
    EnemyBuilding* getBuildingAtPosition(const Position &pos);
    EnemyTower* getTowerAtPosition(const Position &pos);
    void addTower(const EnemyTower &tower);
};

#endif