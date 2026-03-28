#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage_;

public:
    DirectDamageSpell(const std::string& name, const std::string& description, 
                     int damage, int range, int mana_cost = 10);
    
    bool can_cast(const Field& field, int target_x, int target_y) const override;
    bool cast(Field& field, int target_x, int target_y) override;
    int get_damage() const { return damage_; }
    std::string get_short_info() const override { 
        return "Урон: " + std::to_string(damage_); 
    }
    
    SpellType get_type() const override { return SpellType::DirectDamage; }
};

#endif