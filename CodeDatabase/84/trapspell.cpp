#include "trapspell.h"
#include "gamefield.h"
#include <iostream>

TrapSpell::TrapSpell(const std::string& name, 
                    const std::string& description,
                    int damage, int range, int mana_cost)
    : Spell(name, description, range, mana_cost), damage_(damage) {}

bool TrapSpell::can_cast(const Field& field, int target_x, int target_y) const {
    if (!Spell::can_cast(field, target_x, target_y)) {
        return false;
    }
    
    if (!field.can_move_to(target_x, target_y)) return false;
    
    return !field.has_trap_at(target_x, target_y) && 
           !field.has_enemy_at(target_x, target_y) && 
           !field.has_enemy_tower_at(target_x, target_y);
}

bool TrapSpell::cast(Field& field, int target_x, int target_y) {
    if (!can_cast(field, target_x, target_y)) {
        std::cout << "Нельзя установить ловушку в этой позиции!" << std::endl;
        return false;
    }
    
    field.place_trap(target_x, target_y, damage_);
    std::cout << "Ловушка '" << name_ << "' установлена в позиции (" 
              << target_x << ", " << target_y << ")!" << std::endl;
    return true;
}