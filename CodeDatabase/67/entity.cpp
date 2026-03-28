#include "entity.h"

Entity::Entity(int hp, int dmg, int x, int y): health_points(hp), damage(dmg), position_x(x), position_y(y), max_health_points(hp)
{}

int Entity::get_health_points() const{
    return health_points;
}

int Entity::get_damage() const{
    return damage;
}

int Entity::get_position_x() const{
    return position_x;
}

int Entity::get_position_y() const{
    return position_y;
}

int Entity::get_max_health_points() const{
    return max_health_points;
}

void Entity::set_max_health_points(int hp){
    max_health_points = hp;
}

bool Entity::is_dead() const{
    return health_points<=0;
}

void Entity::take_damage(int dmg){
    health_points -= dmg;
}

void Entity::set_health(int hp) {
    if (hp < 0) {
        health_points = 0;
    } else {
        health_points = hp;
    }
}

void Entity::set_damage(int dmg){
    damage = dmg;
}