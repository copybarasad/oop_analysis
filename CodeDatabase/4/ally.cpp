#include "ally.hpp"

Ally::Ally(int id) : Entity(id, 80, 30) {}

Ally::Ally(Ally&& other) noexcept
    : Entity(other.id, other.health, other.damage)
    , skip_turn(other.skip_turn) {}

Ally& Ally::operator=(Ally&& other) noexcept {
    if (this != &other) {
        id = other.id;
        health = other.health;
        damage = other.damage;
        skip_turn = other.skip_turn;
    }
    return *this;
}