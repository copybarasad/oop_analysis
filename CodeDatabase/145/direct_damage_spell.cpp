#include "direct_damage_spell.h"
#include "direct_damage_context.h"
#include "player.h"
#include "game_field.h"
#include "enemy.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "celltype.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>

DirectDamageSpell::DirectDamageSpell(int damage, int radius) 
    : damage(damage), radius(radius) {}

bool DirectDamageSpell::apply(const DirectDamageContext& context) {
    if (!canUse(context)) {
        return false;
    }
    
    int targetX = context.targetX;
    int targetY = context.targetY;
    
    
    for (auto it = context.enemies.begin(); it != context.enemies.end(); ) {
        if (it->getPosition().first == targetX && it->getPosition().second == targetY) {
            std::cout << "Direct damage spell! Hit enemy at (" << targetX << "," << targetY 
                      << ") for " << damage << " damage!\n";
            it->takeDamage(damage);
            
            if (!it->isAlive()) {
                std::cout << "Enemy defeated! +10 points\n";
                context.player.increaseScore(10);
                it = context.enemies.erase(it);
                return true;
            }
            std::cout << "Enemy HP: " << it->getHealth() << "/" << it->getMaxHealth() << "\n";
            return true;
        }
        ++it;
    }
    
    
    for (auto it = context.buildings.begin(); it != context.buildings.end(); ) {
        if (it->getPosition().first == targetX && it->getPosition().second == targetY) {
            std::cout << "Direct damage spell! Hit building at (" << targetX << "," << targetY 
                      << ") for " << damage << " damage!\n";
            it->takeDamage(damage);
            
            if (!it->isAlive()) {
                std::cout << "Building destroyed! +150 points\n";
                context.player.increaseScore(150);
                context.field.setCell(targetX, targetY, CellType::EMPTY);
                it = context.buildings.erase(it);
                return true;
            }
            std::cout << "Building HP: " << it->getHealth() << "\n";
            return true;
        }
        ++it;
    }
    
    
    for (auto it = context.towers.begin(); it != context.towers.end(); ) {
        if (it->getPosition().first == targetX && it->getPosition().second == targetY) {
            std::cout << "Direct damage spell! Hit tower at (" << targetX << "," << targetY 
                      << ") for " << damage << " damage!\n";
            it->takeDamage(damage);
            
            if (!it->isAlive()) {
                std::cout << "Tower destroyed! +200 points\n";
                context.player.increaseScore(200);
                context.field.setCell(targetX, targetY, CellType::EMPTY);
                it = context.towers.erase(it);
                return true;
            }
            std::cout << "Tower HP: " << it->getHealth() << "\n";
            return true;
        }
        ++it;
    }
    
    return false;
}

bool DirectDamageSpell::canUse(const DirectDamageContext& context) const {
    auto playerPos = context.player.getPosition();
    int distance = std::abs(context.targetX - playerPos.first) + 
                   std::abs(context.targetY - playerPos.second);
    
    if (distance > radius || distance == 0) {
        return false;
    }
    
    
    for (const auto& enemy : context.enemies) {
        if (enemy.getPosition().first == context.targetX && enemy.getPosition().second == context.targetY) {
            return true;
        }
    }
    
    for (const auto& building : context.buildings) {
        if (building.getPosition().first == context.targetX && building.getPosition().second == context.targetY) {
            return true;
        }
    }
    
    for (const auto& tower : context.towers) {
        if (tower.getPosition().first == context.targetX && tower.getPosition().second == context.targetY) {
            return true;
        }
    }
    
    return false;
}

const char* DirectDamageSpell::getName() const {
    return "Direct Damage";
}

std::unique_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(damage, radius);
}

int DirectDamageSpell::getDamage() const {
    return damage;
}

int DirectDamageSpell::getRadius() const {
    return radius;
}

void DirectDamageSpell::setRadius(int newRadius) {
    radius = newRadius;
}

void DirectDamageSpell::applyEnhancement() {
    setRadius(getRadius() + 1);
    std::cout << "  [Enhanced] Direct Damage radius increased to " << getRadius() << "\n";
}

std::string DirectDamageSpell::getSerializedData() const {
    std::ostringstream oss;
    oss << "DirectDamage " << damage << " " << radius;
    return oss.str();
}