#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "Enemy.h"
#include "Player.h"
#include "Cell.h"
#include "Position.h"
#include <vector>
#include <memory>
#include <random>

class BuildingManager;
class AllyManager;
class TowerManager;

class EnemyManager {
private:
    std::vector<std::unique_ptr<Enemy>> enemies_;
    mutable std::mt19937 rng_;

public:
    EnemyManager();

    void addEnemy(std::unique_ptr<Enemy> enemy, int x, int y);
    int getEnemyCount() const;
    void removeDeadEnemies();
    std::vector<Enemy*> getEnemies() const;

    void processEnemyMovement(Player& player, const BuildingManager& buildingManager, AllyManager* allyManager, TowerManager* towerManager, const std::vector<std::vector<Cell>>& grid, int width, int height);

    bool hasEnemyOnCell(int x, int y) const;
    Enemy& getEnemyAt(int x, int y) const;

private:
    bool isValidPosition(int x, int y, int width, int height) const;
    bool isCellPassable(const Cell& cell) const;
    bool isCellEmpty(int x, int y, const Player& player, const BuildingManager& buildingManager, AllyManager* allyManager, TowerManager* towerManager, const std::vector<std::vector<Cell>>& grid, int width, int height) const;
    std::vector<Pos> findPathBFS(Pos start, Pos target, const Player& player, 
                                 const BuildingManager& buildingManager,
                                 AllyManager* allyManager, TowerManager* towerManager,
                                 const std::vector<std::vector<Cell>>& grid, 
                                 int width, int height) const;
};

#endif
