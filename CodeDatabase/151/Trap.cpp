#include "Trap.h"
#include "GameField.h"
#include <cmath>
#include <iostream>

TrapSpell::TrapSpell(const std::string& name, int damage, int range, int manaCost)
    : Spell(name, manaCost), baseDamage(damage), currentDamage(damage), range(range) {
}

bool TrapSpell::cast(GameField* field, const Position& casterPos, const Position& targetPos) {
    // Проверяем расстояние
    int dx = std::abs(casterPos.x - targetPos.x);
    int dy = std::abs(casterPos.y - targetPos.y);
    if (dx > range || dy > range) {
        std::cout << "Target is too far! Range: " << range << "\n";
        return false;
    }

    // Проверяем, можно ли разместить ловушку
    if (field->isEmptyForTrap(targetPos)) {
        field->addTrap(TrapOnField(targetPos.x, targetPos.y, currentDamage));
        std::cout << "Trap placed at (" << targetPos.x << ", " << targetPos.y << ")!\n";
        return true;
    }
    else {
        std::cout << "Cannot place trap there - cell is occupied!\n";
        return false;
    }
}

Spell* TrapSpell::clone() const {
    return new TrapSpell(*this);
}

void TrapSpell::upgradeDamage(int additionalDamage) {
    currentDamage += additionalDamage;
    std::cout << "Trap damage increased to " << currentDamage << "!\n";
}

int TrapSpell::getCurrentDamage() const {
    return currentDamage;
}