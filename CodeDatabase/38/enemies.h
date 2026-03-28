#pragma once
#include "enemy.h"
#include "field.h"
#include "logger.h"
#include "player.h"
#include "trap.h"
#include <vector>

class Enemies {
private:
public:
  std::vector<Enemy> enemies;

  Enemies() {}
  void updateEnemies(Field &field, Player &player, std::vector<Trap> &traps,
                     Logger &l);
  void addEnemy(Field &field, std::mt19937 &mt,
                std::uniform_int_distribution<std::mt19937::result_type> &distX,
                std::uniform_int_distribution<std::mt19937::result_type> &distY,
                std::uniform_int_distribution<std::mt19937::result_type>
                    &distEnemySpawner);
};
