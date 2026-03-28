#include "spell_area.hpp"
#include <iostream>
#include "gamefield.hpp"
#include "player.hpp"
#include "enemy.hpp"

bool Area_spell::is_valid_target(Position& player_pos, Position& target_pos) const{
    int dx = std::abs(target_pos.get_x() - player_pos.get_x());
    int dy = std::abs(target_pos.get_y() - player_pos.get_y());
    return (dx <= radius && dy <= radius);
}

void Area_spell::apply_effect(Player& player, Position& target_pos, std::vector<std::unique_ptr<Enemy>>& enemies, Gamefield& field){
    int x0 = target_pos.get_x();
    int y0 = target_pos.get_y();
    std::cout << "Area spell '" << name << "' explodes in x: [" << x0 << "," << x0+1 << "], y: [" << y0 << "," << y0+1 << "]!\n";
    for(auto& enemy: enemies){
        if(!enemy->is_alive()) continue;
        Position ep = enemy->get_position();
        if(ep.get_x() >= x0 && ep.get_x() <= x0 + 1 && ep.get_y() >= y0 && ep.get_y() <= y0 + 1){
            enemy->take_damage(damage);
            std::cout << "Enemy at (" << ep.get_x() << ", " << ep.get_y() << ") hit for " << damage << " damage!\n";
        }
    }
}