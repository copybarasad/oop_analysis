#include "Cell.h"

Cell::Cell(){
    is_there_player = false;
    is_there_enemy = false;
    impassible = false;
    trap = false;
    enemy_tower = false;
}
Cell::~Cell() {}


void Cell::set_player(bool value){
    is_there_player = value;
}

void Cell::set_enemy_tower(bool value){
    enemy_tower = value;
}

void Cell::set_enemy(bool value){
    is_there_enemy = value;
}

void Cell::set_impassible(bool value){
    impassible = value;
}

void Cell::set_trap(bool value){
    trap = value;
}


bool Cell::is_player(){
    return is_there_player;
}

bool Cell::is_enemy(){
    return is_there_enemy;
}

bool Cell::is_impassible(){
    return impassible;
}

bool Cell::is_trap(){
    return trap;
}

bool Cell::is_enemy_tower(){
    return enemy_tower;
}


Saving::CellData Cell::serialize() const{
    return {impassible, is_there_player,is_there_enemy,trap,enemy_tower};
}

void Cell::deserialize(const Saving::CellData& data){
    impassible = data.impassible;
    is_there_player = data.is_there_player;
    is_there_enemy = data.is_there_enemy;
    trap = data.trap;
    enemy_tower = data.enemy_tower;
}


