#include "EnemyTower.hpp"
#include <iostream>
EnemyTower::EnemyTower(int x_pos, int y_pos, int start_level) {
    countdown = 10;
    x = x_pos;
    y = y_pos;
    hp=50;
    level = 1;
    setLevel(start_level);
}

void EnemyTower::set_position(int new_x,int new_y) {
    x=new_x;
    y=new_y;
}

void EnemyTower::setLevel(int new_level) {
    level = new_level;
    hp = hp + (level - 1) * 10;
    countdown = std::max(3, countdown - (level - 1));

    std::cout << "Spawn Tower leveled up to " << level
              << " - HP: " << hp << ", Spawn CD: " << countdown << std::endl;
}

bool EnemyTower::spawn_enemy(int& spawn_x, int& spawn_y, Board& board) {
    if (countdown > 0) {
        countdown--;
        return false;
    }

    std::vector<std::pair<int, int>> positions = {
        {x - 1 , y},
        {x, y - 1}
    };

    for (const auto& pos : positions) {
        int pos_x = pos.first;
        int pos_y = pos.second;

        if (board.can_move_to(pos_x, pos_y) &&
    !board.get_cell(pos_x, pos_y).is_enemy_here() &&
    !board.get_cell(pos_x, pos_y).is_player_here() &&
    !board.get_cell(pos_x, pos_y).is_ally_here() &&
    !board.get_cell(pos_x, pos_y).is_tower_here() &&
    !board.get_cell(pos_x, pos_y).is_attack_tower_here()) {

            spawn_x = pos_x;
            spawn_y = pos_y;
            countdown = 10;
            return true;
        }
    }

    countdown = 1;
    return false;
}

void EnemyTower::take_damage(const int player_damage) {
    int old_hp = hp;
    if (player_damage > hp) {
        hp = 0;
    }
    else {
        hp = hp - player_damage;
    }
    std::cout << "Spawn Tower: " << old_hp << " -> " << hp << " (-" << player_damage << ")" << std::endl;
}

bool EnemyTower::is_death() const {
    return  hp==0;
}

void EnemyTower::get_coords(int& cur_x, int& cur_y){
    cur_x = x;
    cur_y = y;
}
