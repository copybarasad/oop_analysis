#include "character.h"
#include <algorithm>

Character::Character(int character_health, int character_damage, Position start_position)
    : health(character_health), max_health(character_health), damage(character_damage),
      position(start_position), is_alive_flag(true) {}

void Character::check_alive_state() {
    is_alive_flag = (health > 0);
}

void Character::move(const Position& new_position) {
    position = new_position;
}

void Character::take_damage(int damage_amount) {
    health -= damage_amount;
    if (health < 0) health = 0;
    check_alive_state();
}

void Character::heal(int heal_amount) {
    health = std::min(health + heal_amount, max_health);
}

bool Character::is_alive() const {
    return is_alive_flag;
}

int Character::get_health() const {
    return health;
}

int Character::get_max_health() const {
    return max_health;
}

int Character::get_damage() const {
    return damage;
}

Position Character::get_position() const {
    return position;
}

void Character::set_position(const Position& new_position) {
    position = new_position;
}