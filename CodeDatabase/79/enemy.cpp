#include "enemy.h"
#include "player.h"
#include "place.h"
#include <iostream>

Enemy::Enemy(int maxHealth_, int health_, int damage_, int x_, int y_, int points_)
: BasicInfo(maxHealth_, health_, damage_, x_, y_, points_){}

void Enemy::print_current_stats(){
    std::cout << "Creature: Enemy" << '\n';
    BasicInfo::print_current_stats();
}

void Enemy::action(Player& player, Place& place){
    if (!check_health()){
        
        int target_x = player.get_x();
        int target_y = player.get_y();
        int res_x = x;
        int res_y = y;

        if (x < target_x) res_x++;
        else if (x > target_x) res_x--;

        if (y < target_y) res_y++;
        else if (y > target_y) res_y--;

        if (res_x == target_x && res_y == target_y){
            player.damage_recieved(damage);
            return;
        }

        if (place.get_cell(res_x, res_y) == Cell::Type::Obstacle) return;
        if (place.get_cell(res_x, res_y) != Cell::Type::Tower && place.get_cell(res_x, res_y) != Cell::Type::Enemy && place.get_cell(res_x, res_y) != Cell::Type::Enemy_slow){
            if (place.get_cell(x, y) == Cell::Type::Enemy_slow){
                place.change_cell(Cell::Type::Slow, x, y);
            } else {
                place.change_cell(Cell::Type::Empty, x, y);
            }

            if (place.get_cell(res_x, res_y) == Cell::Type::Slow){
                place.change_cell(Cell::Type::Enemy_slow, res_x, res_y);
            } else if (place.get_cell(res_x, res_y) == Cell::Type::Trap){
                place.change_cell(Cell::Type::Enemy, res_x, res_y);
                damage_recieved(8);
                if (check_health()){
                    player.increase_points(get_points());
                } 
            } else {
                place.change_cell(Cell::Type::Enemy, res_x, res_y);
            }
            x = res_x;
            y = res_y;
        }
    }
}

Enemy::~Enemy(){}
