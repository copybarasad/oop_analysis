#include "enhancementSpell.h"
#include "game.h"

std::string EnhancementSpell::getName() const {
  return "Enhancement Spell";
}

void EnhancementSpell::apply(Game& game, int tx, int ty, int px, int py) const {
  (void) tx; (void) ty; (void) px; (void) py;
  game.get_player().increaseEnhancement();
}

std::pair<int, int> EnhancementSpell::findTarget(Game& game, int px, int py, int dx, int dy) const {
  (void)game; (void)dx; (void)dy;
  return {px, py};
}

bool EnhancementSpell::resetsEnhancement() const {
  return false;
}

SpellType EnhancementSpell::getType() const {
  return SpellType::EnhancementSpell;
}