#include "character.hpp"

Character::Character(int character_health, int character_damage, Position start_position):
    health(character_health), max_health(100), damage(character_damage), position(start_position), is_alive_flag(true){}

void Character::check_alive_state(){
    if(health <= 0){
        health = 0;
        is_alive_flag = false;
    }
    else{
        is_alive_flag = true;
    }
    if(health > max_health){
        health = max_health;
    }
}

void Character::move(const Position& new_position){
    position = new_position;
}

void Character::take_damage(int damage_amount){
    health -= damage_amount;
    check_alive_state();
}

void Character::heal(int heal_amount){
    health += heal_amount;
    check_alive_state();
}

bool Character::is_alive() const{
    return is_alive_flag;
}

int Character::get_health() const{
    return health;
}

int Character::get_max_health() const{
    return max_health;
}

int Character::get_damage() const{
    return damage;
}

Position Character::get_position() const{
    return position;
}

void Character::set_position(const Position& new_position){
    position = new_position;
}