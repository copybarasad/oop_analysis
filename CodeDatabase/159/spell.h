#ifndef SPELL_H
#define SPELL_H

#include <string>

enum class SpellType {DirectDamageSpell, AreaDamageSpell, TrapSpell, SummonAllySpell, EnhancementSpell};

class Game;

class Spell {
public:
  virtual ~Spell() = default;
  virtual std::string getName() const = 0;
  virtual void apply(Game& game, int tx, int ty,  int px, int py) const = 0;
  virtual std::pair<int, int> findTarget(Game& game, int px, int py, int dx, int dy) const = 0;
  virtual bool resetsEnhancement() const = 0;
  virtual SpellType getType() const = 0;
};

#endif