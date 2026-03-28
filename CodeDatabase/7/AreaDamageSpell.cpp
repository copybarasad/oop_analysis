#include "AreaDamageSpell.h"
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>

AreaDamageSpell::AreaDamageSpell(std::string name, int cost, int range, int damage)
    : name(name), cost(cost), range(range), damage(damage) {}

void AreaDamageSpell::cast(Player& caster, std::vector<Enemy>& enemies, Field& field, int targetX, int targetY) {
    if (caster.getPoints() < cost) {
        std::cout << "Недостаточно очков для заклинания " << name << "!" << std::endl;
        return;
    }
    int distance = std::max(std::abs(targetX - caster.getX()), std::abs(targetY - caster.getY()));
    if (distance > range) {
        std::cout << "Цель слишком далеко!" << std::endl;
        return;
    }

    std::cout << caster.getName() << " применяет " << name << " в точке (" << targetX << ", " << targetY << ")!" << std::endl;
    
    for (int i = targetX; i < targetX + 2; ++i) {
        for (int j = targetY; j < targetY + 2; ++j) {
            for (auto& enemy : enemies) {
                if (enemy.isAlive() && enemy.getX() == i && enemy.getY() == j) {
                    enemy.takeDamage(damage);
                }
            }
        }
    }

    caster.addPoints(-cost);
}

std::string AreaDamageSpell::getName() const { return name; }
int AreaDamageSpell::getCost() const { return cost; }
int AreaDamageSpell::getRange() const { return range; }

void AreaDamageSpell::save(std::ostream& os) const {
    os << "AreaDamageSpell" << std::endl;
    os << name << std::endl;
    os << cost << " " << range << " " << damage << std::endl;
}

void AreaDamageSpell::load(std::istream& is) {
    std::getline(is, name);
    is >> cost >> range >> damage;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}