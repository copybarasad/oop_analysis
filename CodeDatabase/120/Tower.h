#pragma once

#include "DamageSpell.h"
class DamageSpell;
class GameWorld;
class GameLogic;

class Tower : public Entity {
 public:
  Tower(Position pos, int spellDamage = 5, int spellRange = 4);

  void Update(GameWorld& world, GameLogic& logic);

 private:
  std::unique_ptr<DamageSpell> spell;
  int attackRange = 4;
  int cooldownTurns = 3;
  int sinceLastAttack = 3;
};
