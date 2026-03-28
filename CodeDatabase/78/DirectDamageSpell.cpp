#include "DirectDamageSpell.h"
#include "GameField.h"
#include <iostream>
#include <cmath>

bool DirectDamageSpell::cast_by_player(GameField& field, Player& player, int target_x, int target_y){
    if(!field.is_cell_occupied(target_x, target_y)){
        std::cout << "Coords are not valid or cell is empty" << std::endl;
        return false;
    }
    
    Entity* target = field.get_cell(target_x, target_y).get_entity();
    if(!dynamic_cast<Enemy*>(target) && !dynamic_cast<Tower*>(target)){
        std::cout << "Target is not an enemy or tower!" << std::endl;
        return false;
    }
    
    int dx = player.get_x() - target_x;
    int dy = player.get_y() - target_y;
    if(!can_cast(dx, dy, player.get_mana())){
        std::cout << "Not enough mana! Need " << get_mana_cost() << " mana." << " or target not valid" << std::endl;
        return false;
    }

    target->take_damage(get_damage());
    player.add_mana(-get_mana_cost());
    return true;
}

bool DirectDamageSpell::cast_by_tower(Player& player, float multiplier){
    player.take_damage(get_damage() * multiplier);
    return true;
}

bool DirectDamageSpell::upgrade(){
    level_++;
    damage_ += 30;
    mana_cost_ = std::max(5, mana_cost_ - 5);
    return true;
}