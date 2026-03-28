#include "direct_damage_spell.h"
#include "game.h"
#include "game_field.h"
#include <iostream>
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int dmg, int rng) : damage(dmg), range(rng) {}

std::string DirectDamageSpell::getName() const {
    return "Dagon";
}

std::string DirectDamageSpell::getDescription() const {
    return "Deals " + std::to_string(damage) + " damage to a single target within " + std::to_string(range) + " tiles";
}

bool DirectDamageSpell::canCast(int playerX, int playerY, int targetX, int targetY) const {
    int distance = std::abs(playerX - targetX) + std::abs(playerY - targetY);
    return distance <= range;
}

void DirectDamageSpell::cast(Game& game, int targetX, int targetY) {
    auto field = game.getField();
    auto target = field->getEntityAt(targetX, targetY);
    
    if (target && target->getType() == "Enemy") {
        target->takeDamage(damage);
        std::cout << getName() << " hit " << target->getType() << " for " << damage << " damage!" << std::endl;
        
       
    } else {
        std::cout << getName() << " missed! No enemy at target location." << std::endl;
    }
}

int DirectDamageSpell::getManaCost() const {
    return 20;
}