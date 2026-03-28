#include "spell.h"

Spell::Spell(const std::string& spell_name, int spell_damage, int spell_radius)
    : name(spell_name), damage(spell_damage), radius(spell_radius)
{}

std::string Spell::getName() const {
    return name;
}

int Spell::getDamage() const {
    return damage;
}

int Spell::getRadius() const {
    return radius;
}

void Spell::buffDamage(int amount) {
    damage += amount;
}
