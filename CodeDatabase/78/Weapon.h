#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include <cmath>
#include <iostream>

enum class WeaponType {
    UNARMED,
    FISTS,
    SWORD,
    BOW,
    STAFF
};

class Weapon {
public:
    Weapon(WeaponType type);
    WeaponType get_type() const;

    std::string get_name() const { return name_; }
    int get_damage() const { return damage_value_; }
    int get_range() const { return attack_range_; }
    void set_damage(int damage){ damage_value_ = damage; }
private:
    std::string name_;
    int damage_value_;
    int attack_range_;
};

#endif