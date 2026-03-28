#ifndef ALLYMANAGER_H
#define ALLYMANAGER_H

#include "Ally.h"
#include "Cell.h"
#include <vector>
#include <memory>

class Player;
class EnemyManager;
class BuildingManager;
class TowerManager;

class AllyManager {
private:
    std::vector<std::unique_ptr<Ally>> allies_;

public:
    AllyManager() = default;

    void addAlly(std::unique_ptr<Ally> ally, int x, int y);

    int getAllyCount() const;

    void removeDeadAllies();

    bool hasAllyOnCell(int x, int y) const;

    Ally& getAllyAt(int x, int y) const;

    void processAllyMovement(const Player& player, EnemyManager& enemyManager,
                            const BuildingManager& buildingManager,
                            const TowerManager& towerManager,
                            const std::vector<std::vector<Cell>>& grid, 
                            int width, int height);
    
    const std::vector<std::unique_ptr<Ally>>& getAllies() const { return allies_; }
    
private:
    bool isValidPosition(int x, int y, int width, int height) const;
    bool isCellPassable(const Cell& cell) const;
    bool isCellEmpty(int x, int y, const Player& player, const EnemyManager& enemyManager,
                    const BuildingManager& buildingManager, const TowerManager& towerManager,
                    const std::vector<std::vector<Cell>>& grid, int width, int height) const;
};

#endif

