#pragma once
#include "GameLogic.h"
class TrapSpell : public ISpell {
 public:
  TrapSpell(int damage);
  bool Use(GameWorld& world, GameLogic& logic, const Position& userPos,
           const std::optional<Position>& targetPos) override;
  std::string GetName() const override;

  void ApplyBuff(int power) override;
  bool RequiresTarget() const override;

 private:
  int damage;
};
