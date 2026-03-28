#include "BuildingFactory.h"

std::unique_ptr<EnemyBuilding> BuildingFactory::createEnemyBuilding(const Position &buildPosition, int interval) {
    return std::make_unique<EnemyBuilding>(buildPosition, interval);
}

std::unique_ptr<EnemyTower> BuildingFactory::createEnemyTower(const Position &pos, int hp, int dmg, int rng) {
    return std::make_unique<EnemyTower>(pos, hp, dmg, rng);
}