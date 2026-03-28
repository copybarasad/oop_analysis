#include "../include/Ally.h"
#include <iostream>

Ally::Ally(int hp, int dmg) : damage_(dmg) { hp_ = hp > 0 ? hp : 0; }

int Ally::damage() const { return damage_; }

EntityType Ally::type() const { return EntityType::Ally; }

void Ally::save(std::ostream& os) const {
    Entity::save(os);
    os << damage_ << "\n";
}

void Ally::load(std::istream& is) {
    Entity::load(is);
    is >> damage_;
}
