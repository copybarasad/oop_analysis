#pragma once
#include <vector>
#include "enemy.h"
class EnemyManager {
private:
    std::vector<Enemy> enemies;
public:
    void addEnemy(const Enemy& enemy);
    Enemy* getEnemyAt(int x, int y);
    std::vector<Enemy*> getEnemiesInRadius(int x, int y, int radius);
    void removeDeadEnemies();
};
