#include "DirectDamageSpell.h"
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>

DirectDamageSpell::DirectDamageSpell(std::string name, int cost, int range, int damage)
    : name(name), cost(cost), range(range), damage(damage) {}

void DirectDamageSpell::cast(Player& caster, std::vector<Enemy>& enemies, Field& field, int targetX, int targetY) {
    if (caster.getPoints() < cost) {
        std::cout << "Недостаточно очков для заклинания " << name << "!" << std::endl;
        return;
    }

    int distance = std::max(std::abs(targetX - caster.getX()), std::abs(targetY - caster.getY()));
    if (distance > range) {
        std::cout << "Цель слишком далеко!" << std::endl;
        return;
    }

    bool targetFound = false;
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == targetX && enemy.getY() == targetY) {
            std::cout << caster.getName() << " применяет " << name << " на " << enemy.getName() << "!" << std::endl;
            enemy.takeDamage(damage);
            targetFound = true;
            break;
        }
    }

    if (targetFound) {
        caster.addPoints(-cost);
    } else {
        std::cout << "В указанной клетке нет цели для заклинания!" << std::endl;
    }
}

std::string DirectDamageSpell::getName() const { return name; }
int DirectDamageSpell::getCost() const { return cost; }
int DirectDamageSpell::getRange() const { return range; }

void DirectDamageSpell::save(std::ostream& os) const {
    os << "DirectDamageSpell" << std::endl;
    os << name << std::endl;
    os << cost << " " << range << " " << damage << std::endl;
}

void DirectDamageSpell::load(std::istream& is) {
    std::getline(is, name);
    is >> cost >> range >> damage;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
