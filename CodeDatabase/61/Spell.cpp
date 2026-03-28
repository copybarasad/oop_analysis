#include "Spell.h"

Spell::Spell(int radius, int damage) 
    : baseRadius(radius), baseDamage(damage) {}

void Spell::applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) {
    baseRadius += radiusBonus;
    baseDamage += damageBonus;
}

int Spell::getBaseRadius() const {
    return baseRadius;
}

int Spell::getBaseDamage() const {
    return baseDamage;
}

