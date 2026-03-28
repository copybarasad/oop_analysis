#include "spell_direct_damage.hpp"
#include <iostream>
#include "player.hpp"
#include "enemy.hpp"
#include "gamefield.hpp"

bool Direct_damage_spell::is_valid_target(Position& player_pos, Position& target_pos) const{
    int dx = std::abs(target_pos.get_x() - player_pos.get_x());
    int dy = std::abs(target_pos.get_y() - player_pos.get_y());
    return (dx <= radius && dy <= radius);
}

void Direct_damage_spell::apply_effect(Player& player, Position& target_pos, std::vector<std::unique_ptr<Enemy>>& enemies, Gamefield& field){
    for(auto& enemy: enemies){
        if(enemy->is_alive() && enemy->get_position() == target_pos){
            enemy->take_damage(damage);
            std::cout << "Spell '" << name << "' hit enemy for " << damage << " damage!\n";
            return;
        }
    }
    std::cout << "You missed!\n";
    return;
}