#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "ISpell.h"

class DirectDamageSpell : public ISpell
{
private:
    int damage;
    int range;

public:
    DirectDamageSpell(int dmg = 15, int rng = 3);
    bool apply(GameController &controller, const Position &casterPosition, const Position &targetPos) override;
    const char *name() const override { return "Direct Damage"; }
    SpellType getType() const override { return SpellType::DIRECT_DAMAGE; }
    void getParameters(int& dmg, int& rng, int& width, int& height, int& count, int& power) const override {
        dmg = damage;
        rng = range;
        width = 0;
        height = 0;
        count = 0;
        power = 0;
    }
    void increaseRange(int delta) { range += delta; }
    void increaseDamage(int delta) { damage += delta; }
    
    int getDamage() const { return damage; }
    int getRange() const { return range; }
};

#endif