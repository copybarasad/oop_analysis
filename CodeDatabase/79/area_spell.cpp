#include "area_spell.h"
#include "player.h"
#include "place.h"
#include "enemy.h"
#include "tower.h"
#include "magic_tower.h"
#include <string>

area_spell::area_spell(int _damage, int _cost, int _radius, int _attack_radius):
    damage(_damage), cost(_cost), radius(_radius), attack_radius(_attack_radius){
        if (radius < 0) radius = abs(radius);
        if (radius == 0) radius = 5;
        if (damage < 1) damage = 3;
        if (cost < 0) cost = 8;
        if (attack_radius < 1) attack_radius = 1;
    }

int area_spell::attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    int play_x = player.get_x();
    int play_y = player.get_y();

    if (abs(play_x - target_x) > radius || abs(play_y - target_y) > radius || place.get_cell(target_x, target_y) == Cell::Type::Obstacle){
        std::cout << "Позиция слишком далеко или недоступна, не удалось атаковать" << std::endl;
        return 0;
    }


    int enemy_x;
    int enemy_y;
    for (size_t j = 0; j < enemies.size(); j++) {
        enemy_x = enemies[j].get_x();
        enemy_y = enemies[j].get_y();
        if ((enemy_x - target_x) <= attack_radius && (enemy_y - target_y) <= attack_radius && (enemy_x - target_x) >= 0 && (enemy_y - target_y) >= 0){
            enemies[j].damage_recieved(damage);
            if (enemies[j].check_health()){
                player.increase_points(1);
            }
        }
    }
    enemy_x = tower.get_x();
    enemy_y = tower.get_y();
    if ((enemy_x - target_x) <= attack_radius && (enemy_y - target_y) <= attack_radius && (enemy_x - target_x) >= 0 && (enemy_y - target_y) >= 0){
        tower.damage_recieved(damage);
        if (tower.check_health()){
            player.increase_points(3);
        }
    }
    enemy_x = magic_tower.get_x();
    enemy_y = magic_tower.get_y();
    if ((enemy_x - target_x) <= attack_radius && (enemy_y - target_y) <= attack_radius && (enemy_x - target_x) >= 0 && (enemy_y - target_y) >= 0){
        magic_tower.damage_recieved(damage);
        if (magic_tower.check_health()){
            player.increase_points(3);
        }
    }
    return 5;
}
int area_spell::getCost() const{
    return cost;
}
int area_spell::getRadius() const{}
int area_spell::getType() const{
    return type;
}
std::string area_spell::getName() const{
    return "Огненный шар.";
}
std::string area_spell::getDescription() const{
    return "Запускает огненный шар на клетку и наносит урон всем врагам в области поражения";
}
