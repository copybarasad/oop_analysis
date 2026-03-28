#pragma once
#include "Entity.h"
class Trap : public Entity {
 private:
  int damage;

 public:
  Trap(Position pos, int dmg);
  int GetDamage() const;
};
