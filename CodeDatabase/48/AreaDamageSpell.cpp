#include "lib/AreaDamageSpell.h"
#include "lib/GameField.h"
#include "lib/Player.h"
#include <cmath>
#include <iostream>

AreaDamageSpell::AreaDamageSpell(const std::string& spellName, 
                               int spellDamage, 
                               int spellManaCost, 
                               int spellRange,
                               int spellAreaSize) 
    : name(spellName), 
      manaCost(spellManaCost),
      range(spellRange),
      damage(spellDamage),
      areaSize(spellAreaSize) {
    description = "Deals " + std::to_string(damage) + " damage in a " + 
                 std::to_string(areaSize) + "x" + std::to_string(areaSize) + 
                 " area within range " + std::to_string(range);
}

std::string AreaDamageSpell::getName() const {
    return name;
}

std::string AreaDamageSpell::getDescription() const {
    return description;
}

int AreaDamageSpell::getManaCost() const {
    return manaCost;
}

int AreaDamageSpell::getRange() const {
    return range;
}

bool AreaDamageSpell::canCast(int casterX, int casterY, int targetX, int targetY, GameField* field) const {
    if (!field || !field->isValidPosition(targetX, targetY)) {
        return false;
    }
    
    double distance = std::sqrt(std::pow(targetX - casterX, 2) + std::pow(targetY - casterY, 2));
    return distance <= range;
}

bool AreaDamageSpell::cast(Player& caster, int targetX, int targetY, GameField* field) {
    if (!canCast(caster.getX(), caster.getY(), targetX, targetY, field)) {
        std::cout << "Cannot cast " << name << " on target position!\n";
        return false;
    }
    
    std::cout << caster.getName() << " casts " << name << "!\n";
    
    int enemiesHit = 0;
    for (int dx = -areaSize/2; dx <= areaSize/2; ++dx) {
        for (int dy = -areaSize/2; dy <= areaSize/2; ++dy) {
            int checkX = targetX + dx;
            int checkY = targetY + dy;
            
            if (field->isValidPosition(checkX, checkY) && 
                field->getCell(checkX, checkY).getHasEnemy()) {
                enemiesHit++;
                std::cout << "Enemy at (" << checkX << ", " << checkY << ") receives " << damage << " damage!\n";
            }
        }
    }
    
    if (enemiesHit == 0) {
        std::cout << "The spell hits an empty area.\n";
    } else {
        std::cout << "Total enemies hit: " << enemiesHit << "\n";
    }
    
    return true;
}

SpellCard* AreaDamageSpell::clone() const {
    return new AreaDamageSpell(*this);
}