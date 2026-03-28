#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;

public:
    DirectDamageSpell(const std::string& name, const std::string& desc, int spell_range, int spell_damage);
    
    bool cast(Game_controller& controller, const Position& target) override;
    std::string get_type() const override { return "Direct Damage"; }
    
    int get_damage() const { return damage; }
};

#endif