#ifndef PATHFINDINGSERVICE_H
#define PATHFINDINGSERVICE_H

#include "Position.h"
#include "Direction.h"
#include "MapGrid.h"
#include "EnemyManager.h"

class PathfindingService {
public:
    Direction findBestMoveTowardsTarget(const Position& start, const Position& target, 
                                       const MapGrid& grid, const EnemyManager& enemyManager) const;
private:
    bool isPositionBlocked(const Position& pos, const Position& ignorePos, 
                          const MapGrid& grid, const EnemyManager& enemyManager) const;
};

#endif
