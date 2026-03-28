#ifndef ENHANCEMENTSPELL_H
#define ENHANCEMENTSPELL_H

#include "spell.h"

class EnhancementSpell: public Spell {
public:
  std::string getName() const override;
  void apply(Game& game, int tx, int ty, int px, int py) const override;
  std::pair<int, int> findTarget(Game& game, int px, int py, int dx, int dy) const override;
  bool resetsEnhancement() const;
  SpellType getType() const;
};

#endif