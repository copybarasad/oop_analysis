#ifndef TOWERMANAGER_H
#define TOWERMANAGER_H

#include "EnemyTower.h"
#include <vector>
#include <memory>

class Player;

class TowerManager {
private:
    std::vector<std::unique_ptr<EnemyTower>> towers_;

public:
    TowerManager() = default;

    void addTower(std::unique_ptr<EnemyTower> tower);

    int getTowerCount() const;

    void removeDestroyedTowers();

    bool hasTowerOnCell(int x, int y) const;

    EnemyTower& getTowerAt(int x, int y) const;

    void processTowerAttacks(Player& player);
    
    const std::vector<std::unique_ptr<EnemyTower>>& getTowers() const { return towers_; }
};

#endif

