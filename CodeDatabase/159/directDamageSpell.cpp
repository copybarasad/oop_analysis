#include "directDamageSpell.h"
#include "game.h"

DirectDamageSpell::DirectDamageSpell(int dmg): damage(dmg) {}

std::string DirectDamageSpell::getName() const {
  return "Direct Damage Spell";
}

void DirectDamageSpell::apply(Game& game, int tx, int ty, int px, int py) const {
  (void)px;
  (void)py;
  game.applyDamage(tx, ty, damage);
}

std::pair<int, int> DirectDamageSpell::findTarget(Game& game, int px, int py, int dx, int dy) const {
  int maxDist = 3+game.get_player().getEnhancementLevel();
  for (int dist=1; dist<=maxDist; ++dist) {
    int tx = px+dist*dx;
    int ty = py+dist*dy;
    if (tx < 0 || tx >= game.get_field().getWidth() || ty < 0 || ty >= game.get_field().getHeight()) {
      return {-1, -1};
    }
    Cell::State state = game.get_field().getCell(tx, ty).getState();
    if (state == Cell::Wall) {
      return {-1, -1};
    }
    if (state == Cell::Enemy || state == Cell::EnemyBuilding || state == Cell::EnemyTower) {
      return {tx, ty};
    }
  }
  return {-1, -1};
}

bool DirectDamageSpell::resetsEnhancement() const {
  return true;
}

SpellType DirectDamageSpell::getType() const {
  return SpellType::DirectDamageSpell;
}