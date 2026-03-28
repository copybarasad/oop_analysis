#include "building.h"
#include "game_field.h"
#include "constants.h"
#include "enums.h"
#include <iostream>

Building::Building(const std::string& building_name, int pos_x, int pos_y, Game_field* game_field, int interval, int building_health)
    : x(pos_x), y(pos_y), field(game_field),
    spawn_counter(0), spawn_interval(interval), name(building_name),
    health(building_health), max_health(building_health), destroyed(false){
}

int Building::get_x() const{
    return x;
}

int Building::get_y() const{
    return y;
}

std::string Building::get_name() const{
    return name;
}

void Building::update(){
    if (destroyed) return;
    if (spawn_counter >= spawn_interval){
        spawn_enemy();
        spawn_counter = 0;
    } else{
        spawn_counter++;
    }
}

bool Building::should_spawn_enemy() const{
    return spawn_counter >= spawn_interval;
}

void Building::spawn_enemy(){
    if (destroyed) return;
    std::cout << name << " создает нового врага!\n";
}

int Building::get_health() const{
    return health;
}

int Building::get_max_health() const{
    return max_health;
}

bool Building::is_destroyed() const{
    return destroyed;
}

void Building::take_damage(int damage_amount){
    if (destroyed || damage_amount <= 0) return;
    health -= damage_amount;
    std::cout << name << " получает " << damage_amount << " урона! ";
    std::cout << "Здоровье: " << health << "/" << max_health << "\n";
    if (health <= 0){
        health = 0;
        destroyed = true;
        std::cout << name << " разрушено!\n";
    }
}