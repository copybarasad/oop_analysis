#include "DamageSpell.h"


DamageSpell::DamageSpell(int damage, int range)
    : damage(damage), range(range) {}

bool DamageSpell::Use(GameWorld& world, GameLogic& logic,
                      const Position& userPos,
                      const std::optional<Position>& targetPos) {
  Entity* targetEntity = nullptr;

  if (targetPos) {
    auto findAt = [&](auto& container) -> Entity* {
      for (auto& e : container) {
        if (e && e->IsAlive() && e->GetPosition() == *targetPos) {
          return e.get();
        }
      }
      return nullptr;
    };

    targetEntity = findAt(world.GetEnemies());
    if (!targetEntity) targetEntity = findAt(world.GetSpawners());
    if (!targetEntity) targetEntity = findAt(world.GetTowers());
  } else {
    targetEntity = &world.GetPlayer();
  }

  int buffs = world.GetPlayer().ConsumePendingBuffs();
  if (buffs > 0) ApplyBuff(buffs + 2);

  if (!targetEntity || !targetEntity->IsAlive()) return false;

  Position tpos = targetEntity->GetPosition();
  int dist = std::abs(userPos.x - tpos.x) + std::abs(userPos.y - tpos.y);
  if (dist > range) return false;

  targetEntity->TakeDamage(damage);

  logic.events.Notify(DamageEvent{GetName(),damage,tpos.x,tpos.y});

  if (!targetEntity->IsAlive()) {
    logic.events.Notify(GameMessageEvent{"Target defeated by " + GetName() + "."});
  }

  return true;
}

std::string DamageSpell::GetName() const { return "Fireball"; }

void DamageSpell::ApplyBuff(int power) { range += power; }
bool DamageSpell::RequiresTarget() const { return true; }
