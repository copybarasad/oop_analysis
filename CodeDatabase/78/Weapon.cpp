#include "Weapon.h"

Weapon::Weapon(WeaponType type) {
    switch(type) {
        case WeaponType::SWORD:
            name_ = "Sword";
            damage_value_ = 25;
            attack_range_ = 1;
            break;
        case WeaponType::BOW:
            name_ = "Bow";
            damage_value_ = 15;
            attack_range_ = 5;
            break;
        case WeaponType::STAFF:
            name_ = "Staff";
            damage_value_ = 20;
            attack_range_ = 3;
            break;
        case WeaponType::FISTS:
            name_ = "Fists";
            damage_value_ = 10;
            attack_range_ = 1;
            break;
        default:
            name_ = "Unarmed";
            damage_value_ = 0;
            attack_range_ = 0;
            break;
    }
}

WeaponType Weapon::get_type() const {
    if (name_ == "Sword") return WeaponType::SWORD;
    if (name_ == "Bow") return WeaponType::BOW;
    if (name_ == "Staff") return WeaponType::STAFF;
    return WeaponType::FISTS;
}
