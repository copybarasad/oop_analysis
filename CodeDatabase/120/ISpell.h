#pragma once
#include <memory>
#include <optional>

#include "Position.h"
class GameWorld;
class GameLogic;


class ISpell {
 public:
  virtual ~ISpell() noexcept = default;
  virtual bool Use(GameWorld& world, GameLogic& logic, const Position& userPos,
                   const std::optional<Position>& targetPos) = 0;
  virtual bool RequiresTarget() const = 0;
  virtual std::string GetName() const = 0;
  virtual void ApplyBuff(int power) = 0;
};
