#pragma once
#include <memory>

#include "Enemy.h"

class EnemySpawner : public Entity {
 private:
  int spawnRate;
  int turnCounter;

 public:
  EnemySpawner(const Position& spawnPosition, int interval = 6, int hp = 20);
  std::unique_ptr<Enemy> UpdateAndSpawn();
};
