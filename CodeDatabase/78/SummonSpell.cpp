#include "SummonSpell.h"
#include "GameField.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

bool SummonSpell::cast_by_player(GameField& field, Player& player, int target_x, int target_y){

    if(player.get_mana() < get_mana_cost()){
        std::cout << "Not enough mana! Need " << get_mana_cost() << " mana." << std::endl;
        return false;
    }
    int range_spawn = get_range();

    int spawned = 0;
    int attemts_to_spawn = 10;
    while(spawned < get_ally_count() && attemts_to_spawn-- > 0 ){
        int attempts = 10;
        while(attempts-- > 0){
            int dx = rand() % (range_spawn * 2 + 1) - range_spawn;
            int dy = rand() % (range_spawn * 2 + 1) - range_spawn;

            int x = player.get_x() + dx;
            int y = player.get_y() + dy;

            if(!can_cast(dx, dy, player.get_mana())) continue;
            if(!field.is_cell_passable(x, y)) continue;
            std::unique_ptr<Ally> ally = std::make_unique<Ally>(50, x, y);
            if(field.place_entity(std::move(ally), x, y)){
                spawned++;
                break;
            }
        }
    }

    player.add_mana(-get_mana_cost());
    return spawned != 0;
}

bool SummonSpell::upgrade(){
    level_++;
    ally_count_ += 1;
    mana_cost_ = std::max(5, mana_cost_ - 5);
    return true;
}