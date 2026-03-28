#include "lightning_spell.h"
#include "gamefield.h"
#include <iostream>
#include <cmath>

LightningSpell::LightningSpell(const std::string& name, 
                              const std::string& description,
                              int damage, int range, int stun_duration, int mana_cost)
    : Spell(name, description, range, mana_cost), damage_(damage), stun_duration_(stun_duration) {}

bool LightningSpell::cast(Field& field, int target_x, int target_y) {
    if (!can_cast(field, target_x, target_y)) {
        std::cout << "Заклинание не может быть применено в этой позиции!" << std::endl;
        return false;
    }
    
    std::cout << "Заклинание '" << name_ << "' применяется!" << std::endl;
    bool hit_something = false;
    
    for (int y = target_y - 1; y <= target_y + 1; ++y) {
        for (int x = target_x - 1; x <= target_x + 1; ++x) {
            if (!field.is_valid_position(x, y)) continue;

            if (field.has_enemy_at(x, y) || field.has_enemy_tower_at(x, y)) {
                apply_damage_to_target(field, x, y, damage_);
                hit_something = true;
            }
        }
    }
    
    if (!hit_something) {
        std::cout << "Молния ударила в пустую область!" << std::endl;
        return false;
    }
    
    return true;
}