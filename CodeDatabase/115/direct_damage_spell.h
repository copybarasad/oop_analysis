#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "base_spell.h"

class DirectDamageSpell : public BaseSpell {
private:
    int current_damage;
    int current_range;

public:
    DirectDamageSpell(const std::string& spell_name, int spell_damage, int spell_range, int spell_cost);
    
    bool cast(Player& player, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies) override;
    void apply_enhancement(int enhancement_level) override;
    std::unique_ptr<Spell> clone() const override;
    
    int get_damage() const { return current_damage; }
};

#endif