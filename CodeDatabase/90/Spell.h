#ifndef RPG_SPELL_H_
#define RPG_SPELL_H_

#include "Position.h"
#include <memory>
#include <string>

class Player;
class Game;

class Spell {
public:
  virtual ~Spell() = default;
  virtual std::string name() const = 0;
  virtual bool requiresTarget() const = 0;
  virtual int radius() const = 0;
  virtual int areaWidth() const = 0;
  virtual int areaHeight() const = 0;
  virtual bool use(Game &game, const Position &target) = 0;
};

using SpellPtr = std::unique_ptr<Spell>;

#endif // RPG_SPELL_H_