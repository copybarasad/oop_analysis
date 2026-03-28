#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "base_spell.h"

class AreaDamageSpell : public BaseSpell {
private:
    int base_area_size;
    int current_damage;
    int current_area_size;

public:
    AreaDamageSpell(const std::string& spell_name, int spell_damage, int spell_range, int area_size, int spell_cost);
    
    bool cast(Player& player, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies) override;
    void apply_enhancement(int enhancement_level) override;
    std::unique_ptr<Spell> clone() const override;
};

#endif