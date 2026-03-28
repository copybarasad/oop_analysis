#ifndef ENEMY_TOWER_HPP
#define ENEMY_TOWER_HPP

#include "entity.hpp"
#include "field/field.hpp"

class EnemyTower : public Entity {
    int radius {2};
    int damage;
    int timer {0};
    
public:
    EnemyTower(Field &field, Point position = {0, 0}, int health = 12, int damage = 2);
    Team get_team() const;
    EntityType get_entity_type() const;
    std::string make_step();
    std::string str() {return "enemy tower";}
    
private:
    Entity& use_spell();
};

#endif