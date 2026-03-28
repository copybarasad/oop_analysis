#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "field/point.hpp"
#include <string>

enum class EntityType {
    Player,
    Ally,
    Enemy,
    EnemyBuilding,
    EnemyTower
};

enum class Team {
    Player,
    Enemy,
    Neutral
};

class Field;

class Entity {
protected:
    int hp = 12;
    Point position {0, 0};
    int health {hp};
    bool is_alive {true};
    Field &field;

public:
    Entity(Field &field, Point position = {0, 0}, int health = 12);
    virtual EntityType get_entity_type() const = 0;
    bool is_entity_alive() const;
    int get_health() const;
    int get_max_hp() const;
    virtual Team get_team() const;
    Point get_position() const;
    void set_position(Point pos);
    void take_damage(int damage);
    void restore_health(int amount);
    void set_health(int new_health);
    virtual Entity* get_pending_entity();
    virtual std::string make_step() {return "";}
    virtual ~Entity() = default;
    virtual std::string str() {return "Entity"; }
    Point get_position() {
        return position;
    }
};

#endif