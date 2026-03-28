#ifndef BUILDING_H_
#define BUILDING_H_

#include "enemy.h"

class Building : public Enemy {
public:
    Building(int health, int damage, int x = 0, int y = 0);
    Enemy* SpawnEnemy();

private:
    int health_;
    int damage_;
    int x_;
    int y_;
};

#endif // BUILDING_H_

