#include "hero.hpp"
#include <iostream>

Hero::Hero(const std::string& name, int health, int damage)
    : name_(name), health_(health), damage_(damage) {}

std::string Hero::GetName() const { return this->name_; }
int Hero::GetHealth() const { return this->health_; }
int Hero::GetDamage() const { return this->damage_; }

void Hero::TakeDamage(int amount) {
    if (amount < MIN_DAMAGE) amount = MIN_DAMAGE;
    this->health_ -= amount;
    if (this->health_ < MIN_HEALTH) this->health_ = MIN_HEALTH;
}

void Hero::Attack(Hero& target) {
    std::cout << name_ << " атакует " << target.GetName()
              << " и наносит " << damage_ << " урона!\n";
    target.TakeDamage(this->damage_);
}

bool Hero::IsAlive() const { return this->health_ > MIN_HEALTH; }