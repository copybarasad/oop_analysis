#include "tower.h"

Tower::Tower(unsigned int game_lvl, unsigned int hard_lvl): Character(game_lvl, hard_lvl){
    if(game_lvl < 1) game_lvl = 1;
    if(game_lvl > 3) game_lvl = 3;
    if(hard_lvl < 1) hard_lvl = 1;
    if(hard_lvl > 3) hard_lvl = 3;
    
    type = character_type::tower;
    unsigned int lvl = game_lvl + hard_lvl - 1; //  1,  2,   3,   4,   5
    hp = 5 + lvl;                               //  6,  7,   8,   9,  10
    ranged_damage = 2 + (lvl / 2);              //  2,  3,   3,   4,   4
    attack_range = (5 + lvl) / 2;               //  3,  3,   4,   4,   5
    points = 25 * ((7 * lvl - 3) / 4);          // 25, 50, 100, 150, 200
};

Tower::Tower(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp): 
        Tower(game_lvl, hard_lvl) {
    this->hp = hp;
};

Tower::Tower(const Tower& tower): Character(tower), ranged_damage(tower.ranged_damage),
        attack_range(tower.attack_range), points(tower.points) {};

Tower& Tower::operator= (const Tower& tower){
    type = tower.type;
    hp = tower.hp;
    ranged_damage = tower.ranged_damage;
    attack_range = tower.attack_range;
    points = tower.points;
    return *this;
};

unsigned int Tower::attack(){
    return ranged_damage;
};

unsigned int Tower::range(){
    return attack_range;
};

unsigned int Tower::give_points(){
    return points;
}; 