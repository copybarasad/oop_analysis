#pragma once
#include "Position.h"

class Entity {
 private:
  Position position;
  int health;

 public:
  Entity(const Position& pos, int hp);
  void TakeDamage(int dmg);
  bool IsAlive() const;
  Position GetPosition() const;
  void SetPosition(const Position& pos);
  int GetHealth() const;
  void SetHealth(int hp);
};