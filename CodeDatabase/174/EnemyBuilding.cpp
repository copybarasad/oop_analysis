#include "EnemyBuilding.h"
#include "Constants.h"

EnemyBuilding::EnemyBuilding(int x, int y, int hp, int damage)
    : Unit(hp, damage, x, y) {}
