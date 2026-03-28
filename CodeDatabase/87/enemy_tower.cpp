#include "enemy_tower.hpp"
#include <iostream>

void Enemy_tower::update_cooldown(){
    if(cooldown > 0) cooldown--;
}

void Enemy_tower::attack_player_if_in_range(Player& player, Gamefield& field){
    if(cooldown > 0) return;
    Position player_pos = player.get_position();
    int dx = std::abs(player_pos.get_x() - position.get_x());
    int dy = std::abs(player_pos.get_y() - position.get_y());
    if(dx <= attack_radius && dy <= attack_radius){
        if(!field.is_line_clear(position, player_pos)) return;
        player.take_damage(damage);
        std::cout << "Enemy Tower at (" << position.get_x() << ", " << position.get_y() << ") shoots you for " << damage << " damage!\n";
        cooldown = max_cooldown;
    }
}