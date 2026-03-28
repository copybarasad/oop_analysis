#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "spell.h"

class TrapSpell : public Spell {
private:
    int damage_;

public:
    TrapSpell(const std::string& name, const std::string& description,
             int damage, int range, int mana_cost = 5);
    
    bool can_cast(const Field& field, int target_x, int target_y) const override;
    bool cast(Field& field, int target_x, int target_y) override;
    std::string get_short_info() const override { 
        return "Урон ловушки: " + std::to_string(damage_); 
    }
    SpellType get_type() const override { return SpellType::Trap; }
};

#endif