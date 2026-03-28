#include "summonAllySpell.h"
#include "game.h"

std::string SummonAllySpell::getName() const {
  return "Summon Ally Spell";
}

void SummonAllySpell::apply(Game& game, int tx, int ty, int px, int py) const {
  (void)px; (void)py; (void)tx; (void)ty;
  int level = game.get_player().getEnhancementLevel();
  int count = 1+level;
  std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  int summoned = 0;
  for (size_t i=0; i<directions.size() && summoned<count; ++i) {
    int nx = px+directions[i].first;
    int ny = py+directions[i].second;
    if (nx>=0 && nx<game.get_field().getWidth() && ny>=0 && ny<game.get_field().getHeight() && game.get_field().getCell(nx, ny).getState() == Cell::Empty) {
      game.addAlly(nx, ny);
      summoned++;
    }
  }
}

std::pair<int, int> SummonAllySpell::findTarget(Game& game, int px, int py, int dx, int dy) const {
  (void)dx;
  (void)dy;
  std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  for (auto& direction: directions) {
    int nx = px+direction.first;
    int ny = py+direction.second;
    if (nx>=0 && nx<game.get_field().getWidth() && ny>=0 && ny<game.get_field().getHeight() && game.get_field().getCell(nx, ny).getState() == Cell::Empty) {
      return {nx, ny};
    }
  }
  return {-1, -1};
}

bool SummonAllySpell::resetsEnhancement() const {
  return true;
}

SpellType SummonAllySpell::getType() const {
  return SpellType::SummonAllySpell;
}