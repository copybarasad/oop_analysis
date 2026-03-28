#include "TrapSpell.h"
#include "Player.h"
#include "Field.h"
#include "Node.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>

TrapSpell::TrapSpell(std::string name, int cost, int range, int damage)
    : name(name), cost(cost), range(range), damage(damage) {}

void TrapSpell::cast(Player& caster, std::vector<Enemy>& enemies, Field& field, int targetX, int targetY) {
    if (caster.getPoints() < cost) {
        std::cout << "Недостаточно очков для заклинания " << name << "!" << std::endl;
        return;
    }
    int distance = std::max(std::abs(targetX - caster.getX()), std::abs(targetY - caster.getY()));
    if (distance > range) {
        std::cout << "Слишком далеко, чтобы установить ловушку!" << std::endl;
        return;
    }
    
    if (!field.areCoordinatesValid(targetX, targetY) || field.getNode(targetX, targetY).getType() != Type::EMPTY) {
        std::cout << "Нельзя установить ловушку в занятой или некорректной клетке!" << std::endl;
        return;
    }

    field.getNode(targetX, targetY).setTrap(damage);
    std::cout << "Ловушка '" << name << "' установлена в точке (" << targetX << ", " << targetY << ")!" << std::endl;
    caster.addPoints(-cost);
}

std::string TrapSpell::getName() const { return name; }
int TrapSpell::getCost() const { return cost; }
int TrapSpell::getRange() const { return range; }

void TrapSpell::save(std::ostream& os) const {
    os << "TrapSpell" << std::endl;
    os << name << std::endl;
    os << cost << " " << range << " " << damage << std::endl;
}

void TrapSpell::load(std::istream& is) {
    std::getline(is, name);
    is >> cost >> range >> damage;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}