#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include "constants.h"

class Game_field;

class Building{
protected:
    int x;
    int y;
    Game_field* field;
    int spawn_counter;
    int spawn_interval;
    std::string name;
    int health;
    int max_health;
    bool destroyed;

public:
    Building(const std::string& building_name, int pos_x, int pos_y, Game_field* game_field, int interval = SPAWN_INTERVAL, int building_health = BUILDING_DEFAULT_HEALTH);
    int get_x() const;
    int get_y() const;
    std::string get_name() const;

    //связано с заклинаниями
    int get_health() const;
    int get_max_health() const;
    bool is_destroyed() const;
    void take_damage(int damage_amount);

    virtual void update();
    bool should_spawn_enemy() const;
    void spawn_enemy();
};

#endif