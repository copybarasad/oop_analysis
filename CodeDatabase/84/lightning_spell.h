#ifndef LIGHTNING_SPELL_H
#define LIGHTNING_SPELL_H

#include "spell.h"

class LightningSpell : public Spell {
private:
    int damage_;
    int stun_duration_;

public:
    LightningSpell(const std::string& name, const std::string& description,
                  int damage, int range, int stun_duration = 1, int mana_cost = 20);
    
    bool cast(Field& field, int target_x, int target_y) override;
    std::string get_short_info() const override { 
        return "Урон: " + std::to_string(damage_) + " + Оглушение"; 
    }
    SpellType get_type() const override { return SpellType::Lightning; }
};

#endif