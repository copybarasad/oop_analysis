#include "Ally.hpp"

Ally::Ally() : Character(0, 0, 20, 20, 5) {}

Ally::Ally(int x, int y, int health, int damage)
    : Character(x, y, health, health, damage) 
{}

void Ally::serialize(std::ostream& os) const {
    Character::serialize(os);
}

void Ally::deserialize(std::istream& is) {
    Character::deserialize(is);
}