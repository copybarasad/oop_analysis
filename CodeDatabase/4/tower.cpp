#include "tower.hpp"

Tower::Tower(int number, int temp_level, int temp_id)
    : Entity(temp_id, 150 + 150 * (temp_level - 1), 0)
    , current_timer(number)
    , cooldown(number)
    , level(temp_level) {}

Tower::Tower(Tower&& other) noexcept
    : Entity(other.id, other.health, other.damage)
    , current_timer(other.current_timer)
    , cooldown(other.cooldown)
    , level(other.level) {}

Tower& Tower::operator=(Tower&& other) noexcept {
    if (this != &other) {
        id = other.id;
        health = other.health;
        damage = other.damage;
        current_timer = other.current_timer;
        cooldown = other.cooldown;
        level = other.level;
    }
    return *this;
}