#ifndef TOWERMANAGER_H
#define TOWERMANAGER_H

#include "Position.h"
#include "EnemyTower.h"
#include <vector>
#include <utility>
#include <memory>

class Player;

class TowerManager {
private:
    std::vector<std::pair<Position, std::unique_ptr<EnemyTower>>> towers;

public:
    void addTower(const Position& pos, std::unique_ptr<EnemyTower> tower);
    bool hasTowerAt(const Position& pos) const;
    void processTowers(Player& player, const Position& playerPosition);
    
    int attackTowerAt(const Position& pos, int damage);
    int removeDeadTowers();
    
    const std::vector<std::pair<Position, std::unique_ptr<EnemyTower>>>& getTowers() const;
};

#endif
