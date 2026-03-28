#include "Ally.h"
#include "GameField.h"
#include <iostream>
#include <cmath>
#include <limits>

void Ally::update(GameField& game_field){
    if(is_alive()){
        move_towards_nearest_enemy(game_field);
    }
}

void Ally::move_towards_nearest_enemy(GameField& field){
    int target_x, target_y;
    if(find_nearest_enemy(field, target_x, target_y)){

        if(abs(get_x() - target_x) <= 1 && abs(get_y() - target_y) <= 1){
            Entity* enemy = field.get_cell(target_x, target_y).get_entity();
            if(enemy){
                enemy->take_damage(get_weapon().get_damage());
            }
            return;
        }

        int dx = 0, dy = 0;
        
        if (get_x() < target_x) dx = 1;
        else if (get_x() > target_x) dx = -1;
        if (get_y() < target_y) dy = 1;
        else if (get_y() > target_y) dy = -1;
        
        if (dx != 0 || dy != 0) {
            int attempts = 4;
            int pos_x = get_x() + dx;
            int pos_y = get_y() + dy;
            while(attempts-- > 0){
                if(field.is_cell_passable(pos_x, pos_y) &&
                    (std::abs(pos_x - get_x()) <= 1) &&
                        (std::abs(pos_y - get_y()) <= 1)){
                    field.move_entity(get_x(), get_y(), pos_x, pos_y);
                    set_position(pos_x, pos_y);
                    break;
                }else{
                    int new_dx = std::rand() % 2 - 1; 
                    int new_dy = std::rand() % 2 - 1;
                    if(field.is_cell_passable(pos_x+new_dx, pos_y)) pos_x += new_dx;
                    else if(field.is_cell_passable(pos_x, pos_y+new_dy)) pos_y += new_dy;
                    else if(field.is_cell_passable(pos_x+new_dx, pos_y+new_dy)) pos_x += new_dx; pos_y += new_dy;
                }
            }
        }
    }
}

bool Ally::find_nearest_enemy(GameField& field, int& target_x, int& target_y) const {
    double min_distance = std::numeric_limits<double>::max();
    bool found = false;
    
    const auto& enemies = field.get_enemies();
    for(const auto& enemy : enemies){
        if(enemy->is_alive()){
            double distance = sqrt(pow(get_x() - enemy->get_x(), 2) + pow(get_y() - enemy->get_y(), 2));
            if(distance < min_distance){
                min_distance = distance;
                target_x = enemy->get_x();
                target_y = enemy->get_y();
                found = true;
            }
        }
    }
    
    const auto& towers = field.get_towers();
    for(const auto& tower : towers){
        if(tower->is_alive()){
            double distance = sqrt(pow(get_x() - tower->get_x(), 2) + pow(get_y() - tower->get_y(), 2));
            if(distance < min_distance){
                min_distance = distance;
                target_x = tower->get_x();
                target_y = tower->get_y();
                found = true;
            }
        }
    }
    
    return found;
}

void Ally::show_stats() const {
    std::cout << "=== ALLY STATS ===" << std::endl;
    show_stats();
}