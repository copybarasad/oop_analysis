#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include "Position.h"
#include "Direction.h"
#include "Player.h"
#include "EnemyManager.h"
#include "MapGrid.h"
#include "PathfindingService.h"

class BuildingManager;
class TowerManager;
class AllyManager;

class CombatSystem {
private:
    const PathfindingService& pathfinding;

public:
    CombatSystem(const PathfindingService& pathfinding);
    
    bool movePlayer(Direction direction, Player& player, Position& playerPosition, 
                   const MapGrid& grid, const EnemyManager& enemyManager);
    void moveEnemies(Player& player, const Position& playerPosition, 
                    const MapGrid& grid, EnemyManager& enemyManager,
                    const BuildingManager& buildingManager,
                    const TowerManager& towerManager,
                    AllyManager& allyManager);
    int attackInDirection(Direction direction, int damage, bool isRanged,
                         const Position& playerPosition, const MapGrid& grid,
                         EnemyManager& enemyManager, int& outTotalDamage);
};

#endif
