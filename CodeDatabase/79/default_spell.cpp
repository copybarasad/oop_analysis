#include "default_spell.h"
#include "player.h"
#include "place.h"
#include "enemy.h"
#include "tower.h"
#include "magic_tower.h"
#include <string>

default_spell::default_spell(int _damage, int _cost, int _radius):
    damage(_damage), cost(_cost), radius(_radius){
        if (radius < 0) radius = abs(radius);
        if (radius == 0) radius = 6;
        if (damage < 0) damage = 5;
        if (cost < 0) cost = 3;
    }

int default_spell::attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    int play_x = player.get_x();
    int play_y = player.get_y();
    if (abs(play_x - target_x) > radius || abs(play_y - target_y) > radius){
        std::cout << "Позиция слишком далеко, не удалось атаковать" << std::endl;
        return 0;
    }
    Cell::Type targetType = place.get_cell(target_x, target_y);
    if (targetType != Cell::Type::Enemy && targetType != Cell::Type::Tower && targetType != Cell::Type::Enemy_slow && targetType != Cell::Type::Magic_tower){
        std::cout << "На позиции нет врага или башни, некого атаковать" << std::endl;
        return 0;
    }

    int enemy_x;
    int enemy_y;
    for (size_t j = 0; j < enemies.size(); j++) {
        enemy_x = enemies[j].get_x();
        enemy_y = enemies[j].get_y();
        if (enemy_x == target_x && enemy_y == target_y){
            enemies[j].damage_recieved(damage);
            if (enemies[j].check_health()){
                player.increase_points(1);
                return 5;
            }
            return 1;
        }
    }
    enemy_x = tower.get_x();
    enemy_y = tower.get_y();
    if (enemy_x == target_x && enemy_y == target_y){
        tower.damage_recieved(damage);
        if (tower.check_health()){
            player.increase_points(3);
            return 5;
        }
        return 1;
    }
    enemy_x = magic_tower.get_x();
    enemy_y = magic_tower.get_y();
    if (enemy_x == target_x && enemy_y == target_y){
        magic_tower.damage_recieved(damage);
        if (magic_tower.check_health()){
            player.increase_points(3);
            return 5;
        }
        return 1;
    }
    return 0;
}

int default_spell::getCost() const{
    return cost;
}
int default_spell::getRadius() const{
    return radius;
}
int default_spell::getType() const{
    return type;
}
std::string default_spell::getName() const{
    return "Заклинание прямого урона.";
}
std::string default_spell::getDescription() const{
    return "Наносит прямой единоразовый урон";
}