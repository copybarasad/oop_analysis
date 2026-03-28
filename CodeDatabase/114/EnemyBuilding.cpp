#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int posX, int posY, int turnsToSpawn, int buildingHealth)
    : Entity(buildingHealth, buildingHealth), turnsToSpawn(turnsToSpawn), maxTurnsToSpawn(turnsToSpawn)
{
    setPosition(posX, posY);
}

void EnemyBuilding::update() {
    if (turnsToSpawn > 0) {
        --turnsToSpawn;
    }
}

bool EnemyBuilding::canSpawnEnemy() {
    if (turnsToSpawn <= 0 && isAlive()) {
        turnsToSpawn = maxTurnsToSpawn;
        return true;
    }
    return false;
}
