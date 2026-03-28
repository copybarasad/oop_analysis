#include "EnemyBuilding.h"
#include "GameConfig.h"

EnemyBuilding::EnemyBuilding(const Position &buildPosition, int interval)
    : position(buildPosition), spawnCounter(0), spawnInterval(interval) {}

Position EnemyBuilding::getPosition() const { return position; }
bool EnemyBuilding::shouldSpawnEnemy() const { return spawnCounter>=spawnInterval; }
void EnemyBuilding::update() { spawnCounter++; }

Enemy EnemyBuilding::spawnEnemy() { 
    spawnCounter = 0; 
    Position p(position.x, position.y+1);
    return Enemy(p, 20, 2); 
}