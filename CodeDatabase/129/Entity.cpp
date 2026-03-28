#include "Entity.h"

Entity::Entity() {}

Point Entity::get_coords(){
        return coords;
    }

void Entity::set_coords(int x, int y){
    coords.x = x; coords.y = y;
}

void Entity::shift_left(){
    coords.x--;
}

void Entity::shift_right(){
    coords.x++;
}

void Entity::shift_down(){
    coords.y++;
}

void Entity::shift_up(){
    coords.y--;
}
    
int Entity::get_lifes() {
    return lifes;
}
    
void Entity::take_damage(int damage) {
    lifes -= damage;
    if(lifes < 0) lifes = 0;
}
    
bool Entity::is_alive() {
    if(lifes <= 0 || lifes > 10){
        return false;
    }
    return true;
}

int Entity::get_damage(){
    return damage;
}

Entity::~Entity() {}
