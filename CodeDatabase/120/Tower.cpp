
#include "GameLogic.h"

Tower::Tower(Position pos, int spellDamage, int spellRange) : Entity(pos, 30) {
  spell = std::make_unique<DamageSpell>(spellDamage, spellRange);
}

void Tower::Update(GameWorld& world, GameLogic& logic) {
  if (!IsAlive()) return;

  if (sinceLastAttack < cooldownTurns) {
    ++sinceLastAttack;
    return;
  }

  auto& player = world.GetPlayer();
  Position ppos = player.GetPosition();
  int dist =
      std::abs(ppos.x - GetPosition().x) + std::abs(ppos.y - GetPosition().y);

  if (dist <= attackRange && spell) {
    bool used = spell->Use(world, logic, GetPosition(), std::nullopt);

    if (used) {
      logic.events.Notify(GameMessageEvent{"Enemy tower at (" + std::to_string(GetPosition().x) + "," +
                   std::to_string(GetPosition().y) + ") casts " +
                   spell->GetName() + " on player."});
      sinceLastAttack = 0;
    }
  }
}
