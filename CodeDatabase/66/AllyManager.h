#ifndef ALLYMANAGER_H
#define ALLYMANAGER_H

#include "Position.h"
#include "Ally.h"
#include <vector>
#include <utility>
#include <memory>

class MapGrid;
class EnemyManager;
class PathfindingService;
class BuildingManager;
class TowerManager;

class AllyManager {
private:
    std::vector<std::pair<Position, std::unique_ptr<Ally>>> allies;

public:
    void addAlly(const Position& pos, std::unique_ptr<Ally> ally);
    bool isPositionOccupiedByAlly(const Position& pos) const;
    int removeDeadAllies();
    
    void moveAllies(const Position& playerPosition, const MapGrid& grid, 
                   const EnemyManager& enemyManager, EnemyManager& enemyManagerMutable,
                   const PathfindingService& pathfinding,
                   const BuildingManager& buildingManager,
                   const TowerManager& towerManager);
    
    const std::vector<std::pair<Position, std::unique_ptr<Ally>>>& getAllies() const;
    std::vector<std::pair<Position, std::unique_ptr<Ally>>>& getAllies();
};

#endif
