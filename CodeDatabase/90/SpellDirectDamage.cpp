#include "SpellDirectDamage.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EntityManager.h"
#include "Game.h"
#include "Player.h"

bool SpellDirectDamage::use(Game &game, const Position &target) {
  int dx = target.getX() - game.getPlayer().getPosition().getX();
  int dy = target.getY() - game.getPlayer().getPosition().getY();
  if (dx * dx + dy * dy > effectRadius * effectRadius) {
    return false;
  }

  bool applied = false;

  for (auto *enemy : game.getEnemyManager().getAlive()) {
    if (enemy->getPosition() == target) {
      enemy->takeDamage(damage);
      applied = true;
      break;
    }
  }
  if (!applied) {
    for (auto *b : game.getBuildingManager().getAlive()) {
      if (b->getPosition() == target) {
        b->takeDamage(damage);
        applied = true;
        break;
      }
    }
  }
  return applied;
}
