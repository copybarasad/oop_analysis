#include "fireball_spell.h"
#include "game_field.h"
#include "entity.h"
#include <cmath>
#include <iostream>

FireballSpell::FireballSpell()
    : Spell("Fireball", 20, 4), area_size(2)
{}

bool FireballSpell::canCast(game_field* field, int player_x, int player_y, int target_x, int target_y) const {
    int distance = std::abs(target_x - player_x) + std::abs(target_y - player_y);
    if (distance > radius) {
        return false;
    }
    
    if (!field->isValidPosition(target_x, target_y)) {
        return false;
    }
    
    return true;
}

void FireballSpell::apply(game_field* field, int player_x, int player_y, int target_x, int target_y, Entity* target) {
    std::cout << "Casting " << name << " at (" << target_x << ", " << target_y << ") in " << area_size << "x" << area_size << " area for " << damage << " damage!" << std::endl;
    if (target != nullptr) {
        target->take_damage(damage);
    }
     for (int dy = 0; dy < area_size; dy++) {
        for (int dx = 0; dx < area_size; dx++) {
            int check_x = target_x + dx;
            int check_y = target_y + dy;
            
            if (field->isValidPosition(check_x, check_y)) {
                if (field->get_cell(check_x, check_y).hasEnemy()) {
                    std::cout << "  - Enemy at (" << check_x << ", " << check_y << ") hit by area spell for " << damage << " damage!" << std::endl;
                }
            }
        }
    }
}

int FireballSpell::getAreaSize() const {
    return area_size;
}
