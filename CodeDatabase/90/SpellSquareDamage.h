#ifndef RPG_SPELL_SQUARE_DAMAGE_H_
#define RPG_SPELL_SQUARE_DAMAGE_H_

#include "Spell.h"

class SpellSquareDamage : public Spell {
private:
  int effectRadius;
  int damage;

public:
  SpellSquareDamage(int radius, int damage)
      : effectRadius(radius), damage(damage) {}

  std::string name() const override { return "Square Damage"; }
  bool requiresTarget() const override { return true; }
  int radius() const override { return effectRadius; }
  int areaWidth() const override { return 2; }
  int areaHeight() const override { return 2; }
  bool use(Game &game, const Position &target) override;
};

#endif // RPG_SPELL_SQUARE_DAMAGE_H_
