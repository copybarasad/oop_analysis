#include "area_damage_spell.h"
#include "area_damage_context.h"
#include "player.h"
#include "game_field.h"
#include "enemy.h"
#include "enemy_building.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>

AreaDamageSpell::AreaDamageSpell(int damage, int radius, int areaSize) 
    : damage(damage), radius(radius), areaSize(areaSize) {}

bool AreaDamageSpell::apply(const AreaDamageContext& context) {
    if (!canUse(context)) {
        return false;
    }
    
    int targetX = context.targetX;
    int targetY = context.targetY;
    
    std::cout << "Area damage spell! Hitting " << areaSize << "x" << areaSize 
              << " area at (" << targetX << "," << targetY << ") for " << damage << " damage!\n";
    
    bool hitSomething = false;
    
    
    for (auto it = context.enemies.begin(); it != context.enemies.end(); ) {
        auto pos = it->getPosition();
        if (std::abs(pos.first - targetX) < areaSize && std::abs(pos.second - targetY) < areaSize) {
            it->takeDamage(damage);
            hitSomething = true;
            
            if (!it->isAlive()) {
                std::cout << "  Enemy defeated! +10 points\n";
                context.player.increaseScore(10);
                it = context.enemies.erase(it);
                continue;
            } else {
                std::cout << "  Enemy at (" << pos.first << "," << pos.second 
                          << ") HP: " << it->getHealth() << "\n";
            }
        }
        ++it;
    }
    
    
    for (auto it = context.buildings.begin(); it != context.buildings.end(); ) {
        auto pos = it->getPosition();
        if (std::abs(pos.first - targetX) < areaSize && std::abs(pos.second - targetY) < areaSize) {
            it->takeDamage(damage);
            hitSomething = true;
            
            if (!it->isAlive()) {
                std::cout << "  Building destroyed! +20 points\n";
                context.player.increaseScore(20);
                context.field.setCell(pos.first, pos.second, CellType::EMPTY);
                it = context.buildings.erase(it);
                continue;
            } else {
                std::cout << "  Building at (" << pos.first << "," << pos.second 
                          << ") HP: " << it->getHealth() << "\n";
            }
        }
        ++it;
    }
    
    if (!hitSomething) {
        std::cout << "  No targets in area.\n";
    }
    
    return true;
}

bool AreaDamageSpell::canUse(const AreaDamageContext& context) const {
    auto playerPos = context.player.getPosition();
    int distance = std::abs(context.targetX - playerPos.first) + 
                   std::abs(context.targetY - playerPos.second);
    
    return distance <= radius && context.field.isValidPosition(context.targetX, context.targetY);
}

const char* AreaDamageSpell::getName() const {
    return "Area Damage";
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(damage, radius, areaSize);
}

int AreaDamageSpell::getAreaSize() const {
    return areaSize;
}

void AreaDamageSpell::setAreaSize(int newSize) {
    areaSize = newSize;
}

void AreaDamageSpell::applyEnhancement() {
    setAreaSize(getAreaSize() + 1);
    std::cout << "  [Enhanced] Area size increased to " << getAreaSize() << "x" << getAreaSize() << "\n";
}

int AreaDamageSpell::getDamage() const {
    return damage;
}

int AreaDamageSpell::getRadius() const {
    return radius;
}

std::string AreaDamageSpell::getSerializedData() const {
    std::ostringstream oss;
    oss << "AreaDamage " << damage << " " << radius << " " << areaSize;
    return oss.str();
}