#include "TrapSpell.h"
#include "GameField.h"
#include <iostream>
#include <cmath>

bool TrapSpell::cast_by_player(GameField& field, Player& player, int target_x, int target_y){
    if(!field.is_coordinates_valid(target_x, target_y)){
        std::cout << "Coordinates are not valid" << std::endl;
        return false;
    }

    int dx = target_x - player.get_x();
    int dy = target_y - player.get_y();
    if(!can_cast(dx, dy, player.get_mana())){
        std::cout << "Not enough mana or target out of range! Need " << get_mana_cost() << " mana." << std::endl;
        return false;
    }

    Cell& cell = field.get_cell(target_x, target_y);

    if(!cell.is_passable()){
        std::cout << "Cannot place trap on this cell type" << std::endl;
        return false;
    }

    cell.set_trap();
    cell.set_trap_damage(get_damage());
    player.add_mana(-get_mana_cost());
    return true;
}

bool TrapSpell::upgrade(){
    level_++;
    damage_ += 50;
    mana_cost_ = std::max(5, mana_cost_ - level_ * 5);
    return true;
}