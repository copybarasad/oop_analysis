#ifndef BUILDINGMANAGER_H
#define BUILDINGMANAGER_H

#include "Position.h"
#include "EnemyBuilding.h"
#include "EnemyManager.h"
#include "MapGrid.h"
#include <vector>
#include <memory>

class BuildingManager {
private:
    std::vector<std::pair<Position, std::unique_ptr<EnemyBuilding>>> buildings;

public:
    void addBuilding(const Position& pos, std::unique_ptr<EnemyBuilding> building);
    bool hasBuildingAt(const Position& pos) const;
    void processSpawns(EnemyManager& enemyManager, const MapGrid& grid);
    
    int attackBuildingAt(const Position& pos, int damage);
    int removeDeadBuildings();
    
    const std::vector<std::pair<Position, std::unique_ptr<EnemyBuilding>>>& getBuildings() const;

private:
    int getBuildingIndexAt(const Position& pos) const;
    Position findNearbySpawnPosition(const Position& buildingPos, const MapGrid& grid, const EnemyManager& enemyManager) const;
};

#endif
