#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#include "enemy_building.h"
#include "game_field.h"
#include <vector>

class EnemyManager;
class Player;

class BuildingManager {
private:
    std::vector<EnemyBuilding> buildings;
    GameField& field;
    
public:
    explicit BuildingManager(GameField& field);
    
    // Const геттеры
    const std::vector<EnemyBuilding>& getBuildings() const;
    
    // Не-const геттеры
    std::vector<EnemyBuilding>& getBuildings() { return buildings; }
    
    void addBuilding(const EnemyBuilding& building);
    void processBuildingTurns();
    void spawnEnemiesFromBuildings(EnemyManager& enemyManager, const Player& player);
    void clearBuildings();
};

#endif