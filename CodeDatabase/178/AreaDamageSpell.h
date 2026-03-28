#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "ISpell.h"

class AreaDamageSpell : public ISpell
{
private:
    int damage;
    int width;
    int height;
    int range;

public:
    AreaDamageSpell(int dmg = 10, int w = 2, int h = 2, int rng = 4);
    bool apply(GameController &controller, const Position &casterPosition, const Position &targetPos) override;
    const char *name() const override { return "Area Damage"; }
    SpellType getType() const override { return SpellType::AREA_DAMAGE; }
    void getParameters(int& dmg, int& rng, int& w, int& h, int& count, int& power) const override {
        dmg = damage;
        rng = range;
        w = width;
        h = height;
        count = 0;
        power = 0;
    }
    void increaseArea(int wDelta, int hDelta) { width += wDelta; height += hDelta; }
    void increaseDamage(int delta) { damage += delta; }
    
    int getDamage() const { return damage; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getRange() const { return range; }
};

#endif