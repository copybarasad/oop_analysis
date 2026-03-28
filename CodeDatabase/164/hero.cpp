#include "hero.h"

Hero::Hero(unsigned int game_lvl, unsigned int hard_lvl): Character(game_lvl, hard_lvl){
    if(game_lvl < 1) game_lvl = 1;
    if(game_lvl > 3) game_lvl = 3;
    
    type = character_type::hero;         //  1,  2,  3
    hp = (15 + 5 * game_lvl) / 2;        // 10, 12, 15
    melee_damage = 2 + game_lvl;         //  3,  4,  5
    ranged_damage = 2 + (game_lvl / 2);  //  2,  3,  3
    attack_range = (5 + game_lvl) / 2;   //  3,  3,  4
    money = 0;
    curent_attack = attack_type::melee;
    move = true;
};

Hero::Hero(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp, unsigned int money, 
        bool move): Hero(game_lvl, hard_lvl) {
    this->hp = hp;
    this->money = money;
    this->move = move;
};

Hero::Hero(const Hero& hero): Character(hero), melee_damage(hero.melee_damage),
        ranged_damage(hero.ranged_damage), attack_range(hero.attack_range), money(hero.money),
        curent_attack(hero.curent_attack) {};

Hero& Hero::operator= (const Hero& hero){
    type = hero.type;
    hp = hero.hp;
    melee_damage = hero.melee_damage;
    ranged_damage = hero.ranged_damage;
    attack_range = hero.attack_range;
    money = hero.money;
    curent_attack = hero.curent_attack;
    move = hero.move;
    return *this;
};

void Hero::change_attack(attack_type attack){ // z - smena ataki
    curent_attack = attack;
};

unsigned int Hero::attack(){
    if(curent_attack == attack_type::melee){
        return melee_damage;
    }else if(curent_attack == attack_type::ranged){
        return ranged_damage;
    }
    return 0;
};

unsigned int Hero::range(){
    if(curent_attack == attack_type::melee){
        return 1;
    }else if(curent_attack == attack_type::ranged){
        return attack_range;
    }
    return 0;
};    

void Hero::get_money(unsigned int points){
    money += points;
};

void Hero::change_move(){
    move = !move;
};

bool Hero::win(unsigned int win_points){
    if(money >= win_points) return true;
    return false;
};