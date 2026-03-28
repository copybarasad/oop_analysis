#include "trap_spell.h"
#include "player.h"
#include "place.h"
#include "enemy.h"
#include "tower.h"
#include "magic_tower.h"
#include <string>

Trap_spell::Trap_spell(int _damage, int _cost, int _radius):
    damage(_damage), cost(_cost), radius(_radius){
        if (radius < 0) radius = abs(radius);
        if (radius == 0) radius = 8;
        if (damage < 0) damage = 8;
        if (cost < 0) cost = 4;
    }
int Trap_spell::attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    int play_x = player.get_x();
    int play_y = player.get_y();
    if (abs(play_x - target_x) > radius || abs(play_y - target_y) > radius){
        std::cout << "Позиция слишком далеко, не удалось поставить ловушку" << std::endl;
        return 0;
    }
    Cell::Type targetType = place.get_cell(target_x, target_y);
    if (targetType != Cell::Type::Empty){
        std::cout << "Позиция занята, не удалось поставить ловушку" << std::endl;
        return 0;
    }
    place.change_cell(Cell::Type::Trap, target_x, target_y);
    return 1;
}
int Trap_spell::getCost() const{
    return damage;
}
int Trap_spell::getRadius() const{
    return radius;
}
int Trap_spell::getType() const{
    return type;
}
std::string Trap_spell::getName() const{
    return "Ловушка.";
}
std::string Trap_spell::getDescription() const{
    return "Создаёт на клетке шипы, когда враг наступает на них он получает единоразовый урон.";
}