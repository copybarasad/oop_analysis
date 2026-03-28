#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#include "EnemyBuilding.h"
#include "Enemy.h"
#include "Position.h"
#include "Cell.h"
#include <vector>
#include <memory>

class EnemyManager;
class Player;
class AllyManager;
class TowerManager;

struct BuildingInstance {
    std::unique_ptr<EnemyBuilding> building;
    Pos pos;
};

class BuildingManager {
private:
    std::vector<BuildingInstance> buildings_;

public:
    BuildingManager();

    void addBuilding(std::unique_ptr<EnemyBuilding> building, int x, int y);
    int getBuildingCount() const;
    void updateBuildings(EnemyManager& enemyManager, const Player& player, AllyManager* allyManager, TowerManager* towerManager, const std::vector<std::vector<Cell>>& grid, int width, int height);
    void removeDeadBuildings();

    bool hasBuildingOnCell(int x, int y) const;
    BuildingInstance& getBuildingAt(int x, int y);
    
    const std::vector<BuildingInstance>& getBuildings() const { return buildings_; }

private:
    bool isValidPosition(int x, int y, int width, int height) const;
    bool isCellEmpty(int x, int y, const EnemyManager& enemyManager, const Player& player, AllyManager* allyManager, TowerManager* towerManager, const std::vector<std::vector<Cell>>& grid, int width, int height) const;
};

#endif
