#pragma once
#include "types.hpp"
#include "GameObjects/Entities/Enemy.hpp"
#include "GameObjects/Entities/Tower.hpp"
#include "LevelManager/LMtypes.hpp"

class SpawnTower : public Tower {
public:
    SpawnTower();

    Coordinates getPosition();
    void setPositoin(Coordinates pos);

    void setEnemyStats(EnemyDTO es);
    EnemyDTO getEnemyStats();

    Enemy spawnEnemy(Coordinates pos);
private:
    Coordinates position;
    EnemyDTO enemyStats;
};

