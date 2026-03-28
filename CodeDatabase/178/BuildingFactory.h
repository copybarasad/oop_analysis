#ifndef BUILDING_FACTORY_H
#define BUILDING_FACTORY_H

#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include <memory>

class BuildingFactory {
public:
    static std::unique_ptr<EnemyBuilding> createEnemyBuilding(const Position &buildPosition, int interval = 5);
    static std::unique_ptr<EnemyTower> createEnemyTower(const Position &pos, int hp = 50, int dmg = 10, int rng = 3);
};

#endif