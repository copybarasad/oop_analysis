#ifndef ENEMY_BARRACK_H
#define ENEMY_BARRACK_H

#include "EnemyBuilding.h"
#include "Position.h"
#include <vector>
#include <memory>

class GameField;

class EnemyBarrack : public EnemyBuilding {
private:
    int spawnInterval;
    int currentInterval;

public:
    EnemyBarrack(int x, int y, int cooldown);

    bool canSpawnEnemy();
    Position getSpawnPosition(const GameField& field) const;
    CellType getCellType() const;
};

#endif