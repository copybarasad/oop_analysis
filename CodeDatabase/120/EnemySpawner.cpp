#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(const Position& spawnPosition, int interval, int hp)
    : Entity(spawnPosition, hp), spawnRate(interval), turnCounter(0) {}

std::unique_ptr<Enemy> EnemySpawner::UpdateAndSpawn() {
  turnCounter++;
  if (turnCounter >= spawnRate && IsAlive()) {
    turnCounter = 0;
    return std::make_unique<Enemy>();
  }
  return nullptr;
}
