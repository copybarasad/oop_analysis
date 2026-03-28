#include "TrapSpell.h"


TrapSpell::TrapSpell(int damage) : damage(damage) {}

bool TrapSpell::Use(GameWorld& world, GameLogic& logic, const Position& userPos,
                    const std::optional<Position>& targetPos) {
  (void)userPos;
  int buffs = world.GetPlayer().ConsumePendingBuffs();
  if (buffs > 0) ApplyBuff(buffs + 4);
  if (!targetPos) return false;
  Position t = *targetPos;
  if (!world.GetMap().IsInside(t.x, t.y) ||
      !world.GetMap().IsWalkable(t.x, t.y))
    return false;

  world.AddTrap(std::make_unique<Trap>(t, damage));
  logic.events.Notify(SpellEvent{GetName(),t.x,t.y});
  return true;
}

std::string TrapSpell::GetName() const { return "Trap"; }

void TrapSpell::ApplyBuff(int power) { damage += power; }
bool TrapSpell::RequiresTarget() const { return true; }
