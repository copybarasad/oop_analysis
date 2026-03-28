#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "i_direct_damage_spell.h"

class DirectDamageSpell : public IDirectDamageSpell {
private:
    int damage;
    int range;

public:
    DirectDamageSpell(int dmg = 20, int rng = 3) 
        : damage(dmg), range(rng) {}
    
    bool cast(GameMap& map, Position casterPos, Position target) override;
    std::string getName() const override { return "Прямой урон"; }
    int getRange(int lev) const override { return range * lev; }
};

#endif