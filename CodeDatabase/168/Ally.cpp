#include "Ally.h"

Ally::Ally(int x, int y, int health, int damage)
    : position(x, y), health(health), damage(damage) {
}

void Ally::take_Damage(int dmg) {
    health -= dmg;
    if (health < 0) {
        health = 0;
    }
}

bool Ally::isAlive() const {
    return health > 0;
}

int Ally::get_Health() const {
    return health;
}

int Ally::get_Damage() const {
    return damage;
}

const Position& Ally::get_Position() const {
    return position;
}

void Ally::set_Position(const Position& newPos) {
    position = newPos;
}
