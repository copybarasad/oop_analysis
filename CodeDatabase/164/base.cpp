#include "base.h"

Base::Base(unsigned int game_lvl, unsigned int hard_lvl): Character(game_lvl, hard_lvl){
    if(game_lvl < 1) game_lvl = 1;
    if(game_lvl > 3) game_lvl = 3;
    if(hard_lvl < 1) hard_lvl = 1;
    if(hard_lvl > 3) hard_lvl = 3;
    
    type = character_type::base;
    unsigned int lvl = game_lvl + hard_lvl - 1; //  1,  2,   3,   4,   5
    hp = 5 + lvl;                               //  6,  7,   8,   9,  10
    period_action = 5 - (lvl / 2);              //  5,  4,   4,   3,   3
    period_counter = period_action;
    points = 25 * ((7 * lvl - 3) / 4);          // 25, 50, 100, 150, 200
};

Base::Base(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp): 
        Base(game_lvl, hard_lvl) {
    this->hp = hp;
};

Base::Base(const Base& base): Character(base), period_action(base.period_action),
        period_counter(base.period_counter), points(base.points) {};

Base& Base::operator= (const Base& base){
    type = base.type;
    hp = base.hp;
    period_action = base.period_action;
    period_counter = base.period_counter;
    points = base.points;
    return *this;
};

bool Base::make_enemy(){
    period_counter--;
    if(period_counter == 0){
        period_counter = period_action;
        return true; // sozdat`
    }
    return false; // zhdat`
};

unsigned int Base::give_points(){
    return points;
};