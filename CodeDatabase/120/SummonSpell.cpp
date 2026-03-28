#include "SummonSpell.h"


SummonSpell::SummonSpell(int count) : summonCount(count) {}

bool SummonSpell::Use(GameWorld& world, GameLogic& logic,
                      const Position& userPos,
                      const std::optional<Position>& targetPos) {
  (void)targetPos;
  int buffs = world.GetPlayer().ConsumePendingBuffs();
  if (buffs > 0) ApplyBuff(buffs);
  auto free = world.GetMap().FindFreeAdjacent(userPos);
  int created = 0;
  for (auto& p : free) {
    if (created >= summonCount) break;

    if (world.IsOccupied(p.x, p.y)) continue;
    world.AddSummonedAlly(std::make_unique<SummonedAlly>(p));
    ++created;
    logic.events.Notify(SpellEvent{GetName(),p.x,p.y});
  }
  return created > 0;
}

std::string SummonSpell::GetName() const { return "Ally"; }

void SummonSpell::ApplyBuff(int power) { summonCount += power; }
bool SummonSpell::RequiresTarget() const { return false; }
