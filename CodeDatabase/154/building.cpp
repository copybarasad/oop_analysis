#include "enemy.h"
#include "building.h"

Building::Building(int id, int health, int spawnInterval): Entity(id, health), spawnInterval_(spawnInterval), counter_(spawnInterval) {}
EntityType Building::getType() const { return EntityType::Building; }
int Building::getDamage() const { return 0; }
int Building::getSpawnInterval() const { return spawnInterval_; }
int Building::getCounter() const { return counter_; }
void Building::tick() { if (counter_ > 0) --counter_; }
void Building::resetCounter() { counter_ = spawnInterval_; }

Building* Building::asBuilding() { return this; }

bool Building::shouldSpawn() const { return counter_ <= 0; }
Entity* Building::produceEnemy(int newId) { resetCounter(); return new Enemy(newId, 5, 1); }
