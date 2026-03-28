#include "enemy.h"

Enemy::Enemy(unsigned int game_lvl, unsigned int hard_lvl): Character(game_lvl, hard_lvl){
    if(game_lvl < 1) game_lvl = 1;
    if(game_lvl > 3) game_lvl = 3;
    if(hard_lvl < 1) hard_lvl = 1;
    if(hard_lvl > 3) hard_lvl = 3;
    
    type = character_type::enemy;
    unsigned int lvl = game_lvl + hard_lvl - 1; //  1,  2,  3,  4,  5
    hp = (18 + 11 * lvl) / 4;                   //  7, 10, 12, 15, 18
    melee_damage = (7 + 5 * lvl) / 4;           //  3,  4,  5,  6,  8
    points = 5 * lvl;                           //  5, 10, 15, 20, 25
    move = true;
};

Enemy::Enemy(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp, bool move): 
        Enemy(game_lvl, hard_lvl) {
    this->hp = hp;
    this->move = move;
};


Enemy::Enemy(const Enemy& enemy): Character(enemy), melee_damage(enemy.melee_damage), points(enemy.points) {};

Enemy& Enemy::operator= (const Enemy& enemy){
    type = enemy.type;
    hp = enemy.hp;
    melee_damage = enemy.melee_damage;
    points = enemy.points;
    move = enemy.move;
    return *this;
};

unsigned int Enemy::attack(){
    return melee_damage;
};

unsigned int Enemy::range(){
    return 1;
};   

void Enemy::change_move(){
    move = !move;
};

unsigned int Enemy::give_points(){
    return points;
};