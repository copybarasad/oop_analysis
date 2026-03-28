#include "Enemy.hpp"

Enemy::Enemy (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, unsigned int damageValue) :
    Entity(id, maxHealthValue, currentHealthValue, damageValue) {}

Enemy::Enemy (Enemy& other, size_t id) : Entity(id, other.maxHealth, other.currentHealth, other.damage) {}

unsigned int Enemy::calculateDamage () const { return damage; }