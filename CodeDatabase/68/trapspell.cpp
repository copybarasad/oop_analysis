#include "trapspell.h"
#include "gamefield.h"
#include <iostream>

TrapSpell::TrapSpell(int damage, int manaCost, int range) 
    : damage(damage), manaCost(manaCost), range(range) {}

void TrapSpell::cast(GameField* field, const Position& targetPosition) {
    std::cout << "Casting " << getName() << " at position (" 
              << targetPosition.x << ", " << targetPosition.y << ")\n";
    
    // Lab requirement: "Заклинание размещает на поле ловушку"
    field->placeTrap(targetPosition, damage);
    std::cout << "Trap placed! It will deal " << damage << " damage when triggered.\n";
}

std::string TrapSpell::getName() const {
    return "Trap Spell";
}

int TrapSpell::getManaCost() const {
    return manaCost;
}

int TrapSpell::getDamage() const {
    return damage;
}

int TrapSpell::getRange() const {
    return range;
}

void TrapSpell::upgradeDamage(int additionalDamage) {
    damage += additionalDamage;
    std::cout << "Trap Spell upgraded! Damage: " << damage << "\n";
}

void TrapSpell::upgradeRange(int additionalRange) {
    range += additionalRange;
    std::cout << "Trap Spell range increased! Range: " << range << "\n";
}

void TrapSpell::reduceManaCost(int reduction) {
    manaCost = std::max(1, manaCost - reduction);
    std::cout << "Trap Spell mana cost reduced! Cost: " << manaCost << "\n";
}