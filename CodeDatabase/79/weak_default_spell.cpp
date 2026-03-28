#include "weak_default_spell.h"
#include "player.h"
#include "place.h"
#include "magic_tower.h"
#include <string>

weak_default_spell::weak_default_spell(int _damage, int _radius):
    damage(_damage),  radius(_radius){
        if (radius < 0) radius = abs(radius);
        if (radius == 0) radius = 6;
        if (damage < 0) damage = 5;
    }

int weak_default_spell::attack(Player& player, int target_x, int target_y, Place& place, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    player.damage_recieved(damage);
    return 1;
}
    
int weak_default_spell::getCost() const{
    return 0;
}
int weak_default_spell::getRadius() const{
    return radius;
}
int weak_default_spell::getType() const{
    return 0;
}
std::string weak_default_spell::getName() const{
    return "Weak spell";
}
std::string weak_default_spell::getDescription() const{
    return "Magic tower attack player";
}