#include "building.h"

Building::Building(int x, int y, int interval, int init_health=50)
    : position_x(x),
      position_y(y),
      creating_interval(interval),
      steps_until_create(interval),
      health(init_health) {
        if (health <= 0){
            health = 50;
        }
      }

void Building::did_step() {
    if (steps_until_create > 0) {
        steps_until_create--;
    }
}

bool Building::do_create_enemy() const {
    return steps_until_create == 0;
}

void Building::reset_timer() {
    steps_until_create = creating_interval;
}

int Building::get_x() const {
    return position_x;
}

int Building::get_y() const {
    return position_y;
}

int Building::get_interval() const {
    return creating_interval;
}

void Building::take_damage(int damage_amount) {
    if (damage_amount > 0) {
        health -= damage_amount;
        if (health < 0) health = 0;
    }
}

bool Building::is_destroyed() const {
    return health <= 0;
}

int Building::get_health() const {
    return health;
}