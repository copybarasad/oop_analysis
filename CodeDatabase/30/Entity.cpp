#include "../include/Entity.h"

void Entity::takeDamage(int dmg) {
    hp_ -= dmg;
}

bool Entity::isAlive() const {
    return hp_ > 0;
}

void Entity::save(std::ostream& os) const {
    os << static_cast<int>(type()) << " " << hp_ << "\n";
}

void Entity::load(std::istream& is) {
    is >> hp_;
}