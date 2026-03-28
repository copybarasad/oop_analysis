#include "../include/items/weapon.h"

Weapon::Weapon() : type_(WeaponType::Sword), damage_(15), range_(1), name_("Меч") {}

Weapon::Weapon(WeaponType type) : type_(type) {
    set_type(type);
}

WeaponType Weapon::type() const {
    return type_;
}

int Weapon::damage() const {
    return damage_;
}

int Weapon::range() const {
    return range_;
}

std::string Weapon::name() const {
    return name_;
}

void Weapon::set_type(WeaponType type) {
    type_ = type;
    
    switch (type) {
        case WeaponType::Sword:
            damage_ = 15;
            range_ = 1;
            name_ = "Меч";
            break;
        case WeaponType::Bow:
            damage_ = 10;
            range_ = 3;
            name_ = "Лук";
            break;
    }
}
