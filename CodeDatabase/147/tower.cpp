#include "tower.h"

SpawnerTower::SpawnerTower(int spawnCooldown_, int health)
    : Unit(health, 0), spawnCooldown(spawnCooldown_), currentCooldown(0)
{
    _type = CellType::TOWER;
    if (spawnCooldown < 1) spawnCooldown = 1;
}


void SpawnerTower::tick() {
    if (currentCooldown > 0) --currentCooldown;
}

bool SpawnerTower::canSpawn() const {
    return isAlive() && currentCooldown == 0;
}

void SpawnerTower::resetCooldown() {
    currentCooldown = spawnCooldown;
}
