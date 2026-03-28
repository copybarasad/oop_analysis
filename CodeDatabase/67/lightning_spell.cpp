#include "lightning_spell.h"
#include "game_field.h"
#include <cmath>
#include <iostream>
#include "entity.h"

LightningSpell::LightningSpell()
    : Spell("Lightning", 30, 3)
{}

LightningSpell::LightningSpell(int spell_damage, int spell_radius)
    : Spell("Lightning", spell_damage, spell_radius)
{}

bool LightningSpell::canCast(game_field* field, int player_x, int player_y, int target_x, int target_y) const {
    if (!field->isValidPosition(target_x, target_y)) {
        return false;
    }
    
    int distance = std::abs(target_x - player_x) + std::abs(target_y - player_y);
    if (distance > radius) {
        return false;
    }
    

   if (!field->get_cell(target_x, target_y).hasEnemy() && 
        !field->get_cell(target_x, target_y).hasTower() &&
        !field->get_cell(target_x, target_y).hasPlayer()) {
        return false;
    }
    
    return true;
}

void LightningSpell::apply(game_field* field, int player_x, int player_y, int target_x, int target_y, Entity* target) {
    if (target == nullptr) {
        std::cout << "No target to apply Lightning spell!" << std::endl;
        return;
    }
    
    target->take_damage(damage);

    std::cout << "Casting " << name << " at (" << target_x << ", " << target_y << ") for " << damage << " damage!" << std::endl;
}
