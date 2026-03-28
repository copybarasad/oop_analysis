#include "enemy.hpp"
#include "constants.hpp"

bool Enemy::HasAttacked() const { return has_attacked_; }

Enemy::Enemy(const std::string& name, int health, int damage)
    : Hero(name, health, damage), x_(START_POSITION), y_(START_POSITION) {}

void Enemy::SetPosition(int x, int y) {
    this->x_ = x;
    this->y_ = y;
}

int Enemy::GetX() const { return this->x_; }
int Enemy::GetY() const { return this->y_; }

void Enemy::TakeDamage(int amount) {
    this->health_ -= amount;
    if (this->health_ < MIN_HEALTH) this->health_ = MIN_HEALTH;
    
    std::cout << this->name_ << " получает " << amount
              << " урона. Осталось HP: " << this->health_ << "\n";
}

void Enemy::Attack(Hero& target) {
    std::cout << this->name_ << " атакует " << target.GetName()
              << " и наносит " << this->damage_ << " урона!\n";
    target.TakeDamage(this->damage_);
}