#include "BuffSpell.h"


BuffSpell::BuffSpell() {}

bool BuffSpell::Use(GameWorld& world, GameLogic& logic, const Position& userPos,
                    const std::optional<Position>& targetPos) {
  (void)targetPos;
  (void)userPos;
  world.GetPlayer().AddPendingBuff();
  logic.events.Notify(GameMessageEvent{GetName() + " applied. Next spell(s) will be empowered."});
  return true;
}

std::string BuffSpell::GetName() const { return "Buff"; }

void BuffSpell::ApplyBuff(int power) { (void)power; }
bool BuffSpell::RequiresTarget() const { return false; }
