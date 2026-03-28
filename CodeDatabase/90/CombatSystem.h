#ifndef RPG_COMBAT_SYSTEM_H_
#define RPG_COMBAT_SYSTEM_H_

#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EntityManager.h"
#include "Field.h"
#include "Player.h"

class CombatSystem {
private:
  Player &player;
  EntityManager<Enemy> &enemyManager;
  EntityManager<EnemyBuilding> &buildingManager;
  Field &field;

public:
  CombatSystem(Player &player, EntityManager<Enemy> &enemyManager,
               EntityManager<EnemyBuilding> &buildingManager, Field &field)
      : player(player), enemyManager(enemyManager),
        buildingManager(buildingManager), field(field) {}
  void playerAttack();
  void enemyAttack();
  int cleanup();
};

#endif // RPG_COMBAT_SYSTEM_H_
