#ifndef WEAPON_H
#define WEAPON_H

#include <string>

enum class WeaponType {
    Melee,
    Ranged
};

enum class WeaponName {
    Sword,
    Bow,
    Axe
};

class Weapon {
private:
    std::string name_;
    int damage_;
    WeaponType type_;
    int range_;

public:
    Weapon(WeaponName name, int damage, WeaponType type, int range = 1);
    Weapon(const std::string& name, int damage, WeaponType type, int range = 1);
    
    const std::string& get_name() const;
    int get_damage() const;
    WeaponType get_type() const;
    int get_range() const;
    bool is_melee() const;
    bool is_ranged() const;
};

#endif