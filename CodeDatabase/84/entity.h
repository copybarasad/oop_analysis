#ifndef ENTITY_H
#define ENTITY_H

#include "weapon.h"
#include <iostream>

class Entity {
protected:
    int health_;
    int max_health_;
    int position_x_;
    int position_y_;
    Weapon* weapon_;

public:
    Entity(int health, Weapon* weapon, int x, int y);
    virtual ~Entity() = default;

    void take_damage(int damage);
    bool is_alive() const;
    int get_health() const;
    int get_max_health() const;
    int get_x() const;
    int get_y() const;
    int get_damage() const;
    WeaponType get_attack_type() const;
    const Weapon* get_weapon() const;
    void set_health(int health);
    void set_weapon(Weapon* weapon);
    void move(int dx, int dy);
    bool is_at_position(int x, int y) const;

    virtual void save(std::ostream& os) const;
    virtual void load(std::istream& is);
};

#endif