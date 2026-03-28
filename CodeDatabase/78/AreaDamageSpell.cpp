#include "AreaDamageSpell.h"
#include "GameField.h"
#include "Tower.h"
#include <iostream>
#include <cmath>

bool AreaDamageSpell::cast_by_player(GameField& field, Player& player, int target_x, int target_y){
    int dx = player.get_x() - target_x;
    int dy = player.get_y() - target_y;
    if(!can_cast(dx, dy, player.get_mana())){
        std::cout << "Not enough mana or cast not in range! Need " << get_mana_cost() << " mana." << std::endl;
        return false;
    }

    int hit_count = 0;
    int start_x = target_x - area_size_ / 2;
    int end_x = target_x + area_size_ / 2;
    int start_y = target_y - area_size_ / 2;
    int end_y = target_y + area_size_ / 2;

    for(int y = start_y; y < end_y + 1; y++){
        for(int x = start_x; x < end_x + 1; x++){
            if(field.is_cell_occupied(x, y)){
                Entity* target = field.get_cell(x, y).get_entity();
                if(dynamic_cast<Enemy*>(target) || dynamic_cast<Tower*>(target)){
                    target->take_damage(get_damage());
                    hit_count++;
                }
            }
        }
    }
    player.add_mana(-get_mana_cost());
    
    return true;
}

bool AreaDamageSpell::upgrade(){
    level_++;
    damage_ += 20;
    mana_cost_ = std::max(5, mana_cost_ - 5);
    return true;
}