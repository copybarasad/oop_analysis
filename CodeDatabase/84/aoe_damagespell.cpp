#include "aoe_damagespell.h"
#include "gamefield.h"
#include <iostream>

AoEDamageSpell::AoEDamageSpell(const std::string& name, 
                              const std::string& description,
                              int damage, int range, int area_size, int mana_cost)
    : Spell(name, description, range, mana_cost), damage_(damage), area_size_(area_size) {}

bool AoEDamageSpell::cast(Field& field, int target_x, int target_y) {
    if (!can_cast(field, target_x, target_y)) {
        std::cout << "Заклинание не может быть применено в этой позиции!" << std::endl;
        return false;
    }
    
    std::cout << "Заклинание '" << name_ << "' применяется на область!" << std::endl;
    
    int enemies_hit = 0;
    int towers_hit = 0;
    
    for (int y = target_y - area_size_ / 2; y <= target_y + area_size_ / 2; ++y) {
        for (int x = target_x - area_size_ / 2; x <= target_x + area_size_ / 2; ++x) {
            if (!field.is_valid_position(x, y)) continue;
            
            apply_damage_to_target(field, x, y, damage_);
            
            if (field.has_enemy_at(x, y)) {
                ++enemies_hit;
            }
            if (field.has_enemy_tower_at(x, y)) {
                ++towers_hit;
            }
        }
    }
    
    if (enemies_hit == 0 && towers_hit == 0) {
        std::cout << "В области действия заклинания врагов и башен не обнаружено." << std::endl;
        return false;
    } else {
        std::cout << "Поражено врагов: " << enemies_hit << ", Поражено башен: " << towers_hit << std::endl;
        return true;
    }
}

bool AoEDamageSpell::is_position_in_area(int center_x, int center_y, int x, int y) const {
    return x >= center_x - area_size_ / 2 && x <= center_x + area_size_ / 2 &&
           y >= center_y - area_size_ / 2 && y <= center_y + area_size_ / 2;
}