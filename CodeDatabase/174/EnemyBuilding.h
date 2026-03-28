#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "Unit.h"

class EnemyBuilding : public Unit {
public:
    EnemyBuilding(int x, int y, int hp, int damage);
};

#endif