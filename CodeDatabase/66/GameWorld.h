#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "Position.h"
#include "CellType.h"
#include "Direction.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "MapGrid.h"
#include "EnemyManager.h"
#include "BuildingManager.h"
#include "PathfindingService.h"
#include "CombatSystem.h"
#include "TrapManager.h"
#include "AllyManager.h"
#include "TowerManager.h"
#include <memory>

class GameWorld {
private:
    MapGrid grid;
    Position playerPosition;
    EnemyManager enemyManager;
    BuildingManager buildingManager;
    PathfindingService pathfinding;
    CombatSystem combatSystem;
    TrapManager trapManager;
    AllyManager allyManager;
    TowerManager towerManager;

public:
    GameWorld(int width, int height);
    
    int getWidth() const;
    int getHeight() const;
    Position getPlayerPosition() const;
    CellType getCellType(const Position& pos) const;
    
    void initializeWorld();
    void spawnInitialEnemies(int count = 3, int health = 40, int damage = 10);
    void spawnBuildingsAtDistance(double minDistance);
    void spawnTowersAtDistance(double minDistance);
    
    bool movePlayer(Direction direction, Player& player);
    void processEnemyTurn(Player& player, int& killedEnemiesOut, int& alliesKilledOut);
    int attackInDirection(Direction direction, int damage, bool isRanged, int& outTotalDamage);
    int removeDeadEnemies();
    
    bool hasAliveEnemyAt(const Position& pos) const;
    bool hasBuildingAt(const Position& pos) const;
    bool hasTrapAt(const Position& pos) const;
    bool hasTowerAt(const Position& pos) const;
    bool isPositionOccupiedByAlly(const Position& pos) const;
    
    TrapManager& getTrapManager();
    AllyManager& getAllyManager();
    TowerManager& getTowerManager();
    EnemyManager& getEnemyManager();
    BuildingManager& getBuildingManager();
    const MapGrid& getMapGrid() const;
    
    int attackTargetAt(const Position& pos, int damage);
    
    MapGrid& getGrid();
    EnemyManager& getEnemyManagerNonConst();
    BuildingManager& getBuildingManagerNonConst();
    TowerManager& getTowerManagerNonConst();
    AllyManager& getAllyManagerNonConst();
    TrapManager& getTrapManagerNonConst();
    
    void setPlayerPosition(const Position& pos);
};

#endif
