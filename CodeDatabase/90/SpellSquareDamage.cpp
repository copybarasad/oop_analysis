#include "SpellSquareDamage.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EntityManager.h"
#include "Game.h"

bool SpellSquareDamage::use(Game &game, const Position &target) {
  int dx = target.getX() - game.getPlayer().getPosition().getX();
  int dy = target.getY() - game.getPlayer().getPosition().getY();
  if (dx * dx + dy * dy > effectRadius * effectRadius) {
    return false;
  }

  Position p00 = target;
  Position p10 = Position(target.getX() + 1, target.getY());
  Position p01 = Position(target.getX(), target.getY() + 1);
  Position p11 = Position(target.getX() + 1, target.getY() + 1);

  for (auto *enemy : game.getEnemyManager().getAlive()) {
    const Position &ep = enemy->getPosition();
    if (ep == p00 || ep == p10 || ep == p01 || ep == p11) {
      enemy->takeDamage(damage);
    }
  }
  for (auto *building : game.getBuildingManager().getAlive()) {
    const Position &bp = building->getPosition();
    if (bp == p00 || bp == p10 || bp == p01 || bp == p11) {
      building->takeDamage(damage);
    }
  }

  return true;
}
