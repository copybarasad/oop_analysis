#pragma once
#include "GameLogic.h"
class BuffSpell : public ISpell {
 public:
  ~BuffSpell() noexcept override = default;
  BuffSpell();
  bool Use(GameWorld& world, GameLogic& logic, const Position& userPos,
           const std::optional<Position>& targetPos) override;
  std::string GetName() const override;

  void ApplyBuff(int power) override;
  bool RequiresTarget() const override;
};
