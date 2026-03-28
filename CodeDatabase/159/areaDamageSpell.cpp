#include "areaDamageSpell.h"
#include "game.h"
#include "cell.h"
#include <algorithm>

std::string AreaDamageSpell::getName() const {
  return "Area Damage Spell";
}

void AreaDamageSpell::apply(Game& game, int tx, int ty, int px, int py) const {
  int cx = px;
  int cy = py;

  if (tx!=-1 && ty!=-1) {
    int dx = tx-px;
    int dy = ty-py;
    int dist = std::max(std::abs(dx), std::abs(dy));
    if (dist>0) {
      dx /= dist;
      dy /= dist;

      if (std::abs(dx)>=std::abs(dy)) {
        cx = tx-dx;
        cy = ty;
      } else {
        cx = tx;
        cy = ty-dy;
      }
    }
  }

  int level = game.get_player().getEnhancementLevel();
  int halfSize = level;
  for (int dy2=-halfSize; dy2<halfSize; ++dy2) {
    for (int dx2=-halfSize; dx2<halfSize; dx2++) {
      int nx = cx+dx2;
      int ny = cy+dy2;
      if (nx >= 0 && nx < game.get_field().getWidth() && ny >= 0 && ny < game.get_field().getHeight()) {
        game.applyDamage(nx, ny, 20);
      }
    }
  }
}

std::pair<int, int> AreaDamageSpell::findTarget(Game& game, int px, int py, int dx, int dy) const {
  for (int dist = 1; dist <= 3; ++dist) {
    int tx = px + dist * dx;
    int ty = py + dist * dy;
    if (tx < 0 || tx >= game.get_field().getWidth() || ty < 0 || ty >= game.get_field().getHeight()) {
      return {-1, -1};
    }
    if (game.get_field().getCell(tx, ty).getState() == Cell::Wall) {
      return {-1, -1};
    }
    return {tx, ty};
  }
  return {-1, -1};
}

bool AreaDamageSpell::resetsEnhancement() const {
  return true;
}

SpellType AreaDamageSpell::getType() const {
  return SpellType::AreaDamageSpell;
}