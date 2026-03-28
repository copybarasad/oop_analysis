#include "AreaSpell.h"
#include "../Game.h"
#include <iostream>

AreaSpell::AreaSpell(int spellDamage, int spellRange, int spellAreaSize, const std::string& spellName, int cost)
                : damage(spellDamage), range(spellRange), areaSize(spellAreaSize), name(spellName), manaCost(cost) {
    description = "Deals " + std::to_string(spellDamage) + " damage in " + 
                  std::to_string(areaSize) + "x" + std::to_string(areaSize) + " area";
}

bool AreaSpell::cast(Game& game, const Position& target) const {
    auto area = getAffectedArea(target);
    std::cout << name << " explodes at (" << target.x << ", " << target.y << ")!\n";

    game.damageEnemiesInArea(area, damage);

    return true;
}

std::vector<Position> AreaSpell::getAffectedArea(const Position& center) const {
    std::vector<Position> area;
    
    int startX = center.x - areaSize / 2;
    int startY = center.y - areaSize / 2;

    for (int y = 0; y < areaSize; ++y) {
        for (int x = 0; x < areaSize; ++x) {
            area.emplace_back(startX + x, startY + y);
        }
    }
    return area;
}

bool AreaSpell::canTarget(const Game& game, const Position& casterPos,
                          const Position& target) const {
    if (casterPos.distanceTo(target) > range) {
        return false;
    }
    if (!game.getField().isPositionValid(target)) {
        return false;
    }
    return game.canTargetForAreaSpell(target, range, areaSize);
}

// === Getters === 
std::string AreaSpell::getName() const {
    return name;
}

std::string AreaSpell::getDescription() const {
    return description;
}

int AreaSpell::getManaCost() const {
    return manaCost;
}

int AreaSpell::getRange() const {
    return range;
}

int AreaSpell::getDamage() const {
    return damage;
}

char AreaSpell::getSymbol() const {
    return 'A';     // A - Area
}

void AreaSpell::upgrade() {
    int increase = static_cast<int>(damage * 0.50); // + 50%
    if (increase < 1) increase = 1;
    damage += increase;
}

// === JSON ===
int AreaSpell::getAreaSize() const {
    return areaSize;
}