#pragma once
#include "Entity.h"

class Enemy : public Entity {
 private:
  int damage;

 public:
  Enemy(int hp = 20, int dmg = 5, const Position& pos = {5, 5});
  int GetDamage() const;
  void SetDamage(int dmg);
};
