#include "Entity.h"

#include <iostream>
#include <stdexcept>

Entity::Entity(int health, int damage, int x, int y) 
    : base_health_(health),
      base_damage_(damage),
      health_(health),
      damage_(damage),
      x_(x),
      y_(y), 
      prev_x_(x),
      prev_y_(y),
      was_on_slow_(false),
      slowed_(false) {
    if (health < 0) {
        throw std::invalid_argument("Health must be non-negative");
    }
    if (damage < 0) {
        throw std::invalid_argument("Damage must be non-negative");
    }
    if (x < 0 || y < 0) {
        throw std::invalid_argument("Coordinates cannot be negative");
    }
}

int Entity::GetBaseHealth() const {
    return base_health_;
}

int Entity::GetBaseDamage() const {
    return base_damage_;
}

int Entity::GetHealth() const {
    return health_;
}

int Entity::GetDamage() const {
    return damage_;
}

int Entity::GetX() const {
    return x_;
}

int Entity::GetY() const {
    return y_;
}

int Entity::GetPrevX() const {
    return prev_x_;
}

int Entity::GetPrevY() const {
    return prev_y_;
}

void Entity::SetPosition(int new_x, int new_y) {
    if (new_x < 0 || new_y < 0) {
        throw std::invalid_argument("Coordinates cannot be negative");
    }
    prev_x_ = x_;
    prev_y_ = y_;

    x_ = new_x;
    y_ = new_y;
}

void Entity::SetSlowed(bool value) {
    slowed_ = value;
}

void Entity::SetWasOnSlow(bool value) {
    was_on_slow_ = value; 
}

void Entity::SetHealth(int health) {
    health_ = health;
}
void Entity::SetDamage(int damage) {
    damage_ = damage;
}

void Entity::SetBaseHealth(int health) {
    base_health_ = health;
}
void Entity::SetBaseDamage(int damage) {
    base_damage_ = damage;
}

void Entity::SetPrevPosition(int prev_x, int prev_y) {
    prev_x_ = prev_x;
    prev_y_ = prev_y;
}

bool Entity::IsSlowed() const {
    return slowed_;
}

bool Entity::GetWasOnSlow() const {
     return was_on_slow_; 
}

void Entity::TakeDamage(int damage) {
    health_ -= damage;
    if (health_ < 0) {
        health_ = 0;
    }
}