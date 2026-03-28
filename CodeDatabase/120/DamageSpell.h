#pragma once
#include "GameLogic.h"
class DamageSpell : public ISpell {
 public:
  DamageSpell(int damage, int range);
  bool Use(GameWorld& world, GameLogic& logic, const Position& userPos,
           const std::optional<Position>& targetPos) override;
  std::string GetName() const override;
  void ApplyBuff(int power) override;

  bool RequiresTarget() const override;

 private:
  int damage;
  int range;
};
