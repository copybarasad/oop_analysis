#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "ai_entity.hpp"
#include <cstdlib>

class Enemy : public AIEntity {
    int damage_cnt;
    
public:
    Enemy(Field &field, Point position = {0, 0}, int health = 12, int damage = 2);
    int get_damage() const;
    Entity& enemy_step(Direction direction);
    Team get_team() const;
    EntityType get_entity_type() const;
    std::string make_step();
    std::string str() {return "enemy";}
};

#endif