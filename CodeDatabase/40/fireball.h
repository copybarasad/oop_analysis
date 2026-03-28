#ifndef FIREBALL_H
#define FIREBALL_H

#include "Spell.h"

class Fireball : public Spell {
private:
    int damage;
    int range;

public:
    Fireball (int id, const std::string& name, int cost, int damage, int range);
    
    int getRange() const { return range; }
    
    bool isAreaSpell() const override { return false; }
    void apply(Character& caster, Character& target) const override;
    void applyArea(Character& caster, int centerX, int centerY, Game& game) override;
};

#endif