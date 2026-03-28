#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#include <vector>
#include <functional>
#include "enemybuilding.h"
#include "enemytower.h"
#include "gametypes.h"

class Game;

class BuildingManager {
private:
    std::vector<EnemyBuilding> buildings;
    std::vector<EnemyTower> towers;
    // Callback для уведомления о спавне врага (сущность только информирует, Game решает, что делать)
    std::function<void(int, int, int, int)> enemySpawnCallback;

public:
    void spawnBuildings(Field& field, Player& player, int count,
                        int buildingHealth = 100, int towerHealth = 150,
                        int towerRange = 4, int towerDamage = 8);
    // Установка callback для уведомления о событиях (сущность только информирует)
    void setEnemySpawnCallback(std::function<void(int, int, int, int)> callback);
    void updateBuildings(Field& field, EnemyManager& enemyManager, int enemyHealth, int enemyDamage);
    void updateTowers(Game& game);
    bool isCellOccupiedByBuilding(int x, int y) const;
    
    // Damage buildings and towers
    void damageBuildingAt(int x, int y, int damage);
    void damageTowerAt(int x, int y, int damage);
    
    // Remove destroyed buildings
    void removeDestroyedBuildings();
    
	// Getters
	const std::vector<EnemyBuilding>& getBuildings() const { return buildings; }
	std::vector<EnemyBuilding>& getBuildings() { return buildings; }
	const std::vector<EnemyTower>& getTowers() const { return towers; }
	std::vector<EnemyTower>& getTowers() { return towers; }
};

#endif