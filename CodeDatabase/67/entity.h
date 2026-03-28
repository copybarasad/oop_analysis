#ifndef ENTITY_H
#define ENTITY_H
#include "game_field.h"

class Entity {
protected:
    int health_points;
    int damage;
    int position_x;
    int position_y;
    int max_health_points;

public:
    Entity(int hp, int dmg, int x, int y);
    virtual ~Entity() = default;
    virtual int get_max_health_points() const;
    virtual int get_health_points() const;
    virtual int get_damage() const;
    virtual int get_position_x() const;
    virtual int get_position_y() const;
    virtual void set_position(int x,int y,game_field* field) = 0;
    virtual void take_damage(int damage);
    virtual bool is_dead() const;
    virtual char identification() const = 0;
    virtual void set_health(int hp);
    virtual void set_max_health_points(int hp);
    virtual void set_damage(int dmg);
};

#endif