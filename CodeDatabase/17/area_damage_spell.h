#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "i_area_damage_spell.h"  
#include "game_map.h"             
#include "position.h"             
#include <string>                 

class AreaDamageSpell : public IAreaDamageSpell {
private:
    int damage;
    int range;

public:
    AreaDamageSpell(int dmg = 10, int rng = 3) 
        : damage(dmg), range(rng) {}
    
    bool cast(GameMap& map, Position casterPos, Position target) override;
    std::string getName() const override { return "Область"; }
    int getRange(int lev) const override { return range; }
};

#endif