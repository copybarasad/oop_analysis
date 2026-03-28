#include "trap_spell.h"
#include "game_field.h"
#include <cmath>
#include <iostream>

TrapSpell::TrapSpell()
    : Spell("Trap", 40, 2)
{}

bool TrapSpell::canCast(game_field* field, int player_x, int player_y, int target_x, int target_y) const {
    if (!field->isValidPosition(target_x, target_y)) {
        return false;
    }
    
    int distance = std::abs(target_x - player_x) + std::abs(target_y - player_y);
    if (distance > radius) {
        return false;
    }
    
    if (field->get_cell(target_x, target_y).hasTrap()) {
        return false;
    }
    
    if (field->get_cell(target_x, target_y).hasPlayer() || field->get_cell(target_x, target_y).hasEnemy()) {
        return false;
    }
    
    return true;
}

void TrapSpell::apply(game_field* field, int player_x, int player_y, int target_x, int target_y, Entity* target) {
    field->get_cell(target_x, target_y).setTrap(true, damage);
    std::cout << "Placing " << name << " at (" << target_x << ", " << target_y 
              << ") with " << damage << " damage!" << std::endl;
}