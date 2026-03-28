#pragma once
#include "directdamage.h"
#include "entity.h"
#include "logger.h"
#include "player.h"
#include <cmath>
class Field;

class EnemyTower : public Entity {
private:
  unsigned int range = 1; // Attack range in cells
  bool canAttack;         // Prevent consecutive attacks
  int damage = 20;
  DirectDamage dmg;

public:
  EnemyTower(int x, int y);

  void resetAttack();

  void tryAttack(Player &player, Field &field, DirectDamage &attackSpell,
                 Logger &l);

  void towerSpawner();
};
