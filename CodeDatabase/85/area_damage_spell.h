#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "spell.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int area_size;

public:
    AreaDamageSpell(const std::string& name, const std::string& desc, 
                   int spell_range, int spell_damage, int size = 2);
    
    bool cast(Game_controller& controller, const Position& target) override;
    std::string get_type() const override { return "Area Damage"; }
    
    int get_damage() const { return damage; }
    int get_area_size() const { return area_size; }
};

#endif