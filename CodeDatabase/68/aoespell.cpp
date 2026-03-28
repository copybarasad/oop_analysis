#include "aoespell.h"
#include "gamefield.h"
#include <iostream>
#include <cmath>

AOESpell::AOESpell(int damage, int manaCost, int range) 
    : damage(damage), manaCost(manaCost), range(range) {}

void AOESpell::cast(GameField* field, const Position& targetPosition) {
    std::cout << "Casting " << getName() << " at position (" 
              << targetPosition.x << ", " << targetPosition.y << ")\n";
    
    // Check if target area is within range
    Position playerPos = field->getPlayer()->getPosition();
    int manhattanDistance = abs(playerPos.x - targetPosition.x) + 
                           abs(playerPos.y - targetPosition.y);
    if (manhattanDistance > range) {
        std::cout << "Target out of range! Range: " << range 
                  << ", Distance: " << manhattanDistance << " - Spell not used!\n";
        return;
    }
    
    // Lab requirement: "наносит урон по области 2 на 2 клетки"
    // "Заклинание используется, даже если там нет никого"
    int targetsHit = 0;
    for (int x = targetPosition.x; x < targetPosition.x + 2; x++) {
        for (int y = targetPosition.y; y < targetPosition.y + 2; y++) {
            Position areaPosition(x, y);

            if (areaPosition == playerPos) {
                continue;
            }

            if (field->isValidPosition(areaPosition)) {
                field->applyDamageAt(areaPosition, damage);
                targetsHit++;
            }
        }
    }
    std::cout << "Area damage dealt in 2x2 area! " << targetsHit << " cells affected.\n";
}

std::string AOESpell::getName() const {
    return "AOE Damage Spell";
}

int AOESpell::getManaCost() const {
    return manaCost;
}

int AOESpell::getDamage() const {
    return damage;
}

int AOESpell::getRange() const {
    return range;
}

void AOESpell::upgradeDamage(int additionalDamage) {
    damage += additionalDamage;
    std::cout << "AOE Spell upgraded! Damage: " << damage << "\n";
}

void AOESpell::upgradeRange(int additionalRange) {
    range += additionalRange;
    std::cout << "AOE Spell range increased! Range: " << range << "\n";
}

void AOESpell::reduceManaCost(int reduction) {
    manaCost = std::max(1, manaCost - reduction);
    std::cout << "AOE Spell mana cost reduced! Cost: " << manaCost << "\n";
}