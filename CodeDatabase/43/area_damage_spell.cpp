#include "area_damage_spell.h"
#include "game.h"
#include "game_field.h"
#include <iostream>
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int dmg, int rng) : damage(dmg), range(rng) {}

std::string AreaDamageSpell::getName() const {
    return "Sunstrike";
}

std::string AreaDamageSpell::getDescription() const {
    return "Deals " + std::to_string(damage) + " damage in 2x2 area";
}

bool AreaDamageSpell::canCast(int playerX, int playerY, int targetX, int targetY) const {
    int distance = std::abs(playerX - targetX) + std::abs(playerY - targetY);
    return distance <= range;
}

void AreaDamageSpell::cast(Game& game, int targetX, int targetY) {
    auto field = game.getField();
    std::cout << getName() << " strikes area around (" << targetX << "," << targetY << ")!" << std::endl;
    
    
    int enemiesHit = 0;
    for (int y = targetY - 1; y <= targetY; ++y) {
        for (int x = targetX - 1; x <= targetX; ++x) {
            if (field->isWithinBounds(x, y)) {
                auto entity = field->getEntityAt(x, y);
                if (entity && entity->getType() == "Enemy") {
                    entity->takeDamage(damage);
                    std::cout << getName() << " hit " << entity->getType() << " for " << damage << " damage!" << std::endl;
                    enemiesHit++;
                }
            }
        }
    }
    
    if (enemiesHit == 0) {
        std::cout << getName() << " hit empty area." << std::endl;
    }
}

int AreaDamageSpell::getManaCost() const {
    return 30;
}