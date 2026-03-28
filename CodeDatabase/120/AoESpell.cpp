#include "AoESpell.h"


AoESpell::AoESpell(int damage, int radius) : damage(damage), radius(radius) {}

bool AoESpell::Use(GameWorld& world, GameLogic& logic, const Position& userPos,
                   const std::optional<Position>& targetPos) {
  Position t = *targetPos;
  int dist = std::abs(userPos.x - t.x) + std::abs(userPos.y - t.y);
  if (dist > 7) {
    logic.events.Notify(GameMessageEvent{
        "Cast " + GetName() + " failed: distance must be <= 7"
    });
    return false;
  }
  int buffs = world.GetPlayer().ConsumePendingBuffs();
  if (buffs > 0) ApplyBuff(buffs);
  if (!targetPos) return false;

  int size = radius;
  for (int dy = 0; dy < size; ++dy) {
    for (int dx = 0; dx < size; ++dx) {
      int x = t.x + dx;
      int y = t.y + dy;
      if (!world.GetMap().IsInside(x, y)) continue;
      Player& player = world.GetPlayer();
      if (player.GetPosition().x == x && player.GetPosition().y == y) {
        int playerDamage = damage / 2;
        player.TakeDamage(playerDamage);
        logic.events.Notify(DamageEvent{
                "Player",
                playerDamage,
                x,
                y
            });
      }
      auto applyDamage = [&](auto& group, const std::string& type) {
        for (auto& entity : group) {
          if (entity && entity->IsAlive() && entity->GetPosition().x == x &&
              entity->GetPosition().y == y) {
            entity->TakeDamage(damage);
            logic.events.Notify(DamageEvent{
                        type,
                        damage,
                        x,
                        y
                    });
            if (!entity->IsAlive()) {
              logic.events.Notify(GameMessageEvent{
                            type + " defeated by " + GetName()});
            }
          }
        }
      };
      applyDamage(world.GetEnemies(), "enemy");
      applyDamage(world.GetAllies(), "ally");
      applyDamage(world.GetSpawners(), "spawner");
      applyDamage(world.GetTowers(), "tower");
    }
  }
  return true;
}

std::string AoESpell::GetName() const { return "Meteor"; }

void AoESpell::ApplyBuff(int power) { radius += power; }
bool AoESpell::RequiresTarget() const { return true; }
