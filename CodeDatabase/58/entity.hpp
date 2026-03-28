#pragma once

#include <iostream>
#include "enum_fractions.hpp"

#define BASE_HP 10
#define BASE_DAMAGE 2

class Entity{
private:
    int hp;
    int max_hp{hp};
    int damage;
    int start_damage{damage};
    Fraction fraction;

    int validate_hp(int hp);
    int validate_damage(int damage);

public:
    Entity(int hp = BASE_HP, int damage = BASE_DAMAGE, Fraction fraction = Fraction::RED);
    virtual ~Entity() = default;

    bool is_alive();

    int get_hp();
    void set_hp(int hp);
    
    int attack(Entity& target);

    int get_base_damage();
    void set_base_damage(int damage);

    int get_start_base_damage();
    void set_start_base_demage(int damage);

    int get_max_hp();
    void set_max_hp(int max_hp);

    Fraction get_fraction();

    virtual int get_damage();

    virtual char get_symbol() = 0;
};