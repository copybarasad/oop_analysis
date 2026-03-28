#ifndef AOE_DAMAGE_SPELL_H
#define AOE_DAMAGE_SPELL_H

#include "spell.h"

class AoEDamageSpell : public Spell {
private:
    int damage_;
    int area_size_;

public:
    AoEDamageSpell(const std::string& name, const std::string& description,
                  int damage, int range, int area_size = 2, int mana_cost = 15);
    
    bool cast(Field& field, int target_x, int target_y) override;

    SpellType get_type() const override { return SpellType::AoEDamage; }
    std::string get_short_info() const override { 
        return "Урон: " + std::to_string(damage_) + " (AOE " + std::to_string(area_size_) + ")"; 
    }

private:
    bool is_position_in_area(int center_x, int center_y, int x, int y) const;
};

#endif