#include "trapSpell.h"
#include "game.h"
#include "cell.h"
#include <algorithm>

std::string TrapSpell::getName() const {
  return "Trap Spell";
}

void TrapSpell::apply(Game& game, int tx, int ty, int px, int py) const {
  (void)px; (void)py;
  if (tx != -1 && ty != -1) {
    int level = game.get_player().getEnhancementLevel();
    int damage = 30+level*10;
    game.placeTrap(tx, ty, damage);
  }
}

std::pair<int, int> TrapSpell::findTarget(Game& game, int px, int py, int dx, int dy) const {
  int dist = 1;
  int tx = px + dist * dx;
  int ty = py + dist * dy;

  if (tx < 0 || tx >= game.get_field().getWidth() || ty < 0 || ty >= game.get_field().getHeight()) {
    return {-1, -1};
  }
  
  Cell::State state = game.get_field().getCell(tx, ty).getState();
  if (state == Cell::Wall) {
    return {-1, -1};
  }
  if (state == Cell::Empty) {
    return {tx, ty};
  }
  
  return {-1, -1};
}

bool TrapSpell::resetsEnhancement() const {
  return true;
}

SpellType TrapSpell::getType() const {
  return SpellType::TrapSpell;
}