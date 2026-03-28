#include "Weapon.hpp"

Weapon::Weapon (std::string name, CombatMode combatType, unsigned int maxDamageValue, unsigned int minDamageValue,
    double critChance, unsigned int rangeValue, double critBonusValue) :
    Item(name), type(combatType), maxDamage(maxDamageValue), minDamage(minDamageValue), critChance(critChance), range(rangeValue), critBonus(critBonusValue) {}

void Weapon::setType (CombatMode combatType) { this->type = combatType; }

void Weapon::setMaxDamage (unsigned int maxDamageValue) { maxDamage = maxDamageValue; }

void Weapon::setMinDamage (unsigned int minDamageValue) { minDamage = minDamageValue; }

void Weapon::setCritChance (double critChance) { this->critChance = critChance; }

void Weapon::setRange (unsigned int rangeValue) { range = rangeValue; }

CombatMode Weapon::getType () const { return type; }

unsigned int Weapon::getRange () const { return range; }

unsigned int Weapon::getMinDamage() const { return minDamage; }

unsigned int Weapon::getMaxDamage() const { return maxDamage; }

double Weapon::getCritChance() const { return critChance; }

double Weapon::getCritBonus() const { return critBonus; }

unsigned int Weapon::calculateDamage () {
    std::random_device rd;
    std::mt19937 gen(rd());

    unsigned int damage = minDamage + (gen() % (maxDamage - minDamage + 1));
    
    double chance = (double)(gen() % 100) / 100;
    if (chance < critChance) {
        damage *= critBonus;
    }

    return damage;
}