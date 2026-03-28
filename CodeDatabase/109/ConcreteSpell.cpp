#include "ConcreteSpell.h"

ConcreteSpell::ConcreteSpell(SpellType type, const std::string& name, int damage, int range, int area, int manaCost)
    : type(type), name(name), damage(damage), range(range), area(area), manaCost(manaCost) {}

SpellType ConcreteSpell::getType() const {
    return type;
}

std::string ConcreteSpell::getName() const {
    return name;
}

int ConcreteSpell::getDamage() const {
    return damage;
}

int ConcreteSpell::getRange() const {
    return range;
}

int ConcreteSpell::getArea() const {
    return area;
}

int ConcreteSpell::getManaCost() const {
    return manaCost;
}

void ConcreteSpell::enhanceDamage(int amount) {
    damage += amount;
}

void ConcreteSpell::enhanceRange(int amount) {
    range += amount;
}

void ConcreteSpell::enhanceArea(int amount) {
    area += amount;
}

void ConcreteSpell::enhanceDuration(int amount) {
}

void ConcreteSpell::enhanceSummonCount(int amount) {
}