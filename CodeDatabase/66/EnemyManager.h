#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "Position.h"
#include "Enemy.h"
#include <vector>
#include <memory>

class EnemyManager {
private:
    std::vector<std::pair<Position, std::unique_ptr<Enemy>>> enemies;

public:
    void addEnemy(const Position& pos, std::unique_ptr<Enemy> enemy);
    int attackEnemyAt(const Position& pos, int damage);
    int removeDeadEnemies();
    bool hasAliveEnemyAt(const Position& pos) const;
    bool isPositionOccupiedByEnemy(const Position& pos) const;
    
    const std::vector<std::pair<Position, std::unique_ptr<Enemy>>>& getEnemies() const;
    std::vector<std::pair<Position, std::unique_ptr<Enemy>>>& getEnemies();
    
private:
    int getEnemyIndexAt(const Position& pos) const;
};

#endif
