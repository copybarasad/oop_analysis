#include "Spell.h"

Spell::Spell(const std::string& name, int manaCost)
    : name(name), manaCost(manaCost) {
}

std::string Spell::getName() const {
    return name;
}

int Spell::getManaCost() const {
    return manaCost;
}