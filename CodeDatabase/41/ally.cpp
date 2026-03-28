#include "ally.h"
#include "game_field.h"
#include "enemy.h"
#include "constants.h"
#include <iostream>
#include <cmath>


Ally::Ally(const std::string& ally_name, Game_field* game_field)
    : Player(ally_name, game_field), is_ally(true){
        set_health(ALLY_DEFAULT_HEALTH);
        set_mana(0);
}

void Ally::update(){
    if (!is_alive() || !field) return;

    auto my_pos = field->get_ally_position(this);
    int my_x = my_pos.first;
    int my_y = my_pos.second;
    if (my_x == -1 || my_y == -1) return;

    auto enemy_positions = field->get_enemy_positions();
    if (enemy_positions.empty()) return;
    
    int nearest_x = -1, nearest_y = -1;
    double min_distance = 1000.0;

    for (const auto& enemy_pos : enemy_positions) {
        double distance = std::sqrt(std::pow(enemy_pos.first - my_x, 2) + std::pow(enemy_pos.second - my_y, 2));
        if (distance < min_distance){
            min_distance = distance;
            nearest_x = enemy_pos.first;
            nearest_y = enemy_pos.second;
        }
    }
    if (min_distance <= ALLY_ATTACK_RANGE){
        std::cout << get_name() << " атакует врага в позиции (" << nearest_x << ", " << nearest_y << ")!\n";
        field->perform_attack(my_x, my_y, nearest_x, nearest_y);
        return;
    }
    move_towards_enemy(nearest_x, nearest_y);
}

void Ally::move_towards_enemy(int enemy_x, int enemy_y){
    auto my_pos = field->get_ally_position(this); // получение своей позиции
    int my_x = my_pos.first;
    int my_y = my_pos.second;
    int dx = enemy_x - my_x;
    int dy = enemy_y - my_y;
    int move_x = 0, move_y = 0;

    if (std::abs(dx) > std::abs(dy)){
        move_x = (dx > 0) ? 1 : -1;
    } else{
        move_y = (dy > 0) ? 1 : -1;
    }
    field->move_ally(this, my_x + move_x, my_y + move_y);
}