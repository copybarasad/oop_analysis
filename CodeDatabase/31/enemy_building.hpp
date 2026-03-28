#ifndef ENEMY_BUILDING_HPP
#define ENEMY_BUILDING_HPP

#include "entity.hpp"

class EnemyBuilding : public Entity { 
    int enemy_hp;
    int enemy_damage;
    int period = 50;
    int timer = period;
    Entity* pending_enemy;
    
public:
    EnemyBuilding(Field &field, Point position = {0, 0}, int period = 50, int health = 12, int enemy_hp = 12, int enemy_damage = 2);
    Point create_enemy();
    std::string make_step();
    Entity* get_pending_entity();
    Team get_team() const;
    EntityType get_entity_type() const;
    int get_enemy_hp() const;
    int get_enemy_damage() const;
    std::string str() {return "enemy building";}
};

#endif