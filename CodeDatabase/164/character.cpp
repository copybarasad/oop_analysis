#include "character.h"

Character::Character(unsigned int game_lvl = 1, unsigned int hard_lvl = 1): 
        type(character_type::not_defined), hp(0), move(false) {};

Character::Character(const Character& character): type(character.type), hp(character.hp),
        move(character.move) {};

Character& Character::operator= (const Character& character){
    type = character.type;
    hp = character.hp;
    move = character.move;
    return *this;
};

bool Character::get_damage(unsigned int damage){
    if(hp < damage){
        hp = 0;
    }else{
        hp -= damage;
    }
    return hp > 0; // true - living, false - dead
};

bool Character::can_move(){
    return move;
};

character_type Character::get_type(){
    return type;
};

virtual void Character::change_move(){};

virtual unsigned int Character::give_points(){
    return 0;
};

int Character::get_hp(){
    return hp;
};