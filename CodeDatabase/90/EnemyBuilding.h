#ifndef RPG_ENEMY_BUILDING_H_
#define RPG_ENEMY_BUILDING_H_

#include "Entity.h"
#include "EntityManager.h"
#include "Position.h"
#include <memory>
#include <vector>

class Enemy;
class Player;
class Field;

class EnemyBuilding : public Entity {
private:
  int spawnCooldown;
  int maxCooldown;
  EntityManager<Enemy> *enemies;

public:
  EnemyBuilding(int health, Position position = Position(0, 0),
                int spawnCooldown = 0, EntityManager<Enemy> *enemies = nullptr)
      : Entity(0, health, position), spawnCooldown(spawnCooldown),
        maxCooldown(spawnCooldown), enemies(enemies) {}
  void update(int fieldWidth, int fieldHeight, const Player &player,
              const Field &field);
  bool canSpawnEnemy() const { return spawnCooldown <= 0; }
  int getSpawnCooldown() const { return spawnCooldown; }
  int getMaxCooldown() const { return maxCooldown; }
  void setSpawnCooldown(int spawnCooldown) { spawnCooldown = spawnCooldown; }
  void setMaxCooldown(int maxCooldown) { maxCooldown = maxCooldown; }
};

#endif // RPG_ENEMY_BUILDING_H_