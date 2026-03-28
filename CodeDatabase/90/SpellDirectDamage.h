#ifndef RPG_SPELL_DIRECT_DAMAGE_H_
#define RPG_SPELL_DIRECT_DAMAGE_H_

#include "Spell.h"

class SpellDirectDamage : public Spell {
private:
  int effectRadius;
  int damage;

public:
  SpellDirectDamage(int radius, int damage)
      : effectRadius(radius), damage(damage) {}

  std::string name() const override { return "Direct Damage"; }
  bool requiresTarget() const override { return true; }
  int radius() const override { return effectRadius; }
  int areaWidth() const override { return 1; }
  int areaHeight() const override { return 1; }
  bool use(Game &game, const Position &target) override;
};

#endif // RPG_SPELL_DIRECT_DAMAGE_H_
