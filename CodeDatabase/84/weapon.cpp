#include "weapon.h"

Weapon::Weapon(WeaponName name, int damage, WeaponType type, int range)
    : damage_(damage), type_(type), range_(range) {
    switch(name) {
        case WeaponName::Sword: name_ = "Sword"; break;
        case WeaponName::Bow: name_ = "Bow"; break;
        case WeaponName::Axe: name_ = "Axe"; break;
        default: name_ = "Unknown"; break;
    }
}

Weapon::Weapon(const std::string& name, int damage, WeaponType type, int range)
    : name_(name), damage_(damage), type_(type), range_(range) {}

const std::string& Weapon::get_name() const {
    return name_;
}

int Weapon::get_damage() const {
    return damage_;
}

WeaponType Weapon::get_type() const {
    return type_;
}

int Weapon::get_range() const {
    return range_;
}

bool Weapon::is_melee() const {
    return type_ == WeaponType::Melee;
}

bool Weapon::is_ranged() const {
    return type_ == WeaponType::Ranged;
}