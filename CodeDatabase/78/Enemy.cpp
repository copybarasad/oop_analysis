#include "GameField.h"
#include <iostream>
#include <cmath>

void Enemy::move(GameField& game_field, Player& player) {
    
    if(abs(get_x() - player.get_x()) <= 1 && abs(get_y() - player.get_y()) <= 1){
        player.take_damage(get_weapon().get_damage());
        return;
    }

    int dx = 0, dy = 0;
    
    if (get_x() < player.get_x()) dx = 1;
    else if (get_x() > player.get_x()) dx = -1;
    if (get_y() < player.get_y()) dy = 1;
    else if (get_y() > player.get_y()) dy = -1;
    
    if (dx != 0 || dy != 0) {
        int attempts = 10;
        int pos_x = get_x() + dx;
        int pos_y = get_y() + dy;
        while(attempts-- > 0){
            if(game_field.is_cell_passable(pos_x, pos_y) &&
                    (std::abs(pos_x - get_x()) <= 1) &&
                     (std::abs(pos_y - get_y()) <= 1)){
                game_field.move_entity(get_x(), get_y(), pos_x, pos_y);
                set_position(pos_x, pos_y);
                Cell& cell = game_field.get_cell(pos_x, pos_y);
                if(cell.get_type() == CellType::TRAP){
                    take_damage(cell.get_trap_damage());
                    cell.set_type(CellType::EMPTY);
                    cell.set_trap_damage(0);
                }
                break;
            }else{
                int new_dx = std::rand() % 2 - 1; 
                int new_dy = std::rand() % 2 - 1;
                if(game_field.is_cell_passable(pos_x+new_dx, pos_y)) pos_x += new_dx;
                else if(game_field.is_cell_passable(pos_x, pos_y+new_dy)) pos_y += new_dy ;
                else if(game_field.is_cell_passable(pos_x+new_dx, pos_y+new_dy)) pos_x += new_dx; pos_y += new_dy;
            }
        }
    }
}

void Enemy::update(GameField& game_field) {
    if(is_alive()){
        auto player = game_field.get_player();
        if(player){
            move(game_field, *player);
        }
    }
}

void Enemy::show_stats() const {
    std::cout << "=== ENEMY STATS ===" << std::endl;
    Entity::show_stats();
}