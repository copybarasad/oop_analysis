#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "Enemy.h"
#include <memory>

class EnemyBuilding {
private:
    int spawnCounter;
    int spawnInterval;
    int enemyHealth;
    int enemyDamage;

public:
    EnemyBuilding(int interval, int health, int damage);
    EnemyBuilding(const EnemyBuilding& other);
    
    bool canSpawnEnemy() const;
    void update();
    std::unique_ptr<Enemy> spawnEnemy();
};

#endif
