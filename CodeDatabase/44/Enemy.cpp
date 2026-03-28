#include "Enemy.h"
#include <iostream>
#include <cstdlib>

Enemy::Enemy(int health, int damage, int x, int y)
    : Entity(health, damage, x, y) {}

void Enemy::Attack(Entity& target) {
    if (&target == this)
        throw std::logic_error("Enemy cannot attack itself");

    std::cout << "Враг атакует и наносит " << damage_ << " единицы урона!\n";
    target.TakeDamage(damage_);
}

void Enemy::TakeSpellDamage(int damage, bool can_slow) {
    TakeDamage(damage);
    if (can_slow) {
        SetSlowed(true);
        std::cout << "Враг получил " << damage << (damage > 1 ? " единицы" : " единицу") << " урона и оглушён на 1 ход!\n";
    } else {
        std::cout << "Враг получил " << damage << (damage > 1 ? " единицы" : " единицу") << " урона!\n";
    }
}

std::string Enemy::GetDescription() const {
    return "Враг: HP = " + std::to_string(GetHealth())
        + ", pos = (" + std::to_string(GetX()) + "," + std::to_string(GetY()) + ")";
}