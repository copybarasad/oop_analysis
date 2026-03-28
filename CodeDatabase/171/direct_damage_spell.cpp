#include "03_direct_damage_spell.h"
#include "enemy.h" 
#include <cmath>
#include <iostream>
#include <limits>

DirectDamageSpell::DirectDamageSpell(int damage, int range, int cost)
    : damage_(damage), range_(range), cost_(cost), currentTarget_(nullptr) {
}

bool DirectDamageSpell::cast(const Position& casterPos, const Position& targetPos) {
    int distance = std::abs(casterPos.x - targetPos.x) + std::abs(casterPos.y - targetPos.y);
    if (distance > range_) {
        std::cout << "Цель слишком далеко! Дистанция: " << distance
            << ", максимальная: " << range_ << std::endl;
        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return false;
    }

    if (!currentTarget_) {
        std::cout << "Нет цели для заклинания!" << std::endl;
        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return false;
    }

    // Наносим урон цели
    int oldHealth = currentTarget_->getHealth();
    currentTarget_->takeDamage(damage_);

    std::cout << "Заклинание прямого урона '" << getName()
        << "' наносит " << damage_ << " урона цели!" << std::endl;
    std::cout << "Здоровье цели: "
        << currentTarget_->getHealth() << std::endl;

    return true;
}

std::string DirectDamageSpell::getName() const {
    return "Молния";
}

std::string DirectDamageSpell::getDescription() const {
    return "Наносит " + std::to_string(damage_) +
        " урона одной цели в радиусе " + std::to_string(range_) +
        " клеток. Стоимость: " + std::to_string(cost_);
}

int DirectDamageSpell::getRange() const {
    return range_;
}

int DirectDamageSpell::getCost() const {
    return cost_;
}

bool DirectDamageSpell::canUse() const {
    return true;
}

void DirectDamageSpell::setTargetEnemy(Enemy* enemy) {
    currentTarget_ = enemy;
}