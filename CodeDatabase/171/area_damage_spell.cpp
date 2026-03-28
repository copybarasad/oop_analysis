#include "04_area_damage_spell.h"
#include "enemy.h" 
#include <cmath>
#include <iostream>
#include <limits>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int cost)
    : damage_(damage), range_(range), cost_(cost) {
}

bool AreaDamageSpell::cast(const Position& casterPos, const Position& targetPos) {
    int distance = std::abs(casterPos.x - targetPos.x) + std::abs(casterPos.y - targetPos.y);
    if (distance > range_) {
        std::cout << "Цель слишком далеко! Дистанция: " << distance
            << ", максимальная: " << range_ << std::endl;
        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return false;
    }

    auto area = getArea(targetPos);

    std::cout << "Заклинание урона по площади '" << getName()
        << "' наносит " << damage_ << " урона по области 2x2" << std::endl;
    std::cout << "Центр области: (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;

    std::cout << "Затронутые клетки: ";
    for (const auto& pos : area) {
        std::cout << "(" << pos.x << ", " << pos.y << ") ";
    }
    std::cout << std::endl;

    // Наносим урон всем врагам в области
    bool hitAnyEnemy = false;
    for (auto enemy : enemiesInArea_) {
        if (enemy) {
            int oldHealth = enemy->getHealth();
            enemy->takeDamage(damage_);
            hitAnyEnemy = true;

            std::cout << "Враг в (" << enemy->getPosition().x << ", "
                << enemy->getPosition().y << ") получает урон!" << std::endl;
            std::cout << "Здоровье врага: "
                << enemy->getHealth() << std::endl;

            if (!enemy->isAlive()) {
                std::cout << "Враг повержен!" << std::endl;
            }
        }
    }

    if (!hitAnyEnemy) {
        std::cout << "В области нет врагов" << std::endl;
    }

    return true;
}

std::string AreaDamageSpell::getName() const {
    return "Огненный шар";
}

std::string AreaDamageSpell::getDescription() const {
    return "Наносит " + std::to_string(damage_) +
        " урона по области 2x2 в радиусе " + std::to_string(range_) +
        " клеток. Стоимость: " + std::to_string(cost_);
}

int AreaDamageSpell::getRange() const {
    return range_;
}

int AreaDamageSpell::getCost() const {
    return cost_;
}

bool AreaDamageSpell::canUse() const {
    return true;
}

std::vector<Position> AreaDamageSpell::getArea(const Position& center) const {
    std::vector<Position> area;

    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            area.push_back({ center.x + dx, center.y + dy });
        }
    }

    return area;
}

void AreaDamageSpell::setEnemiesInArea(const std::vector<Enemy*>& enemies) {
    enemiesInArea_ = enemies;
}