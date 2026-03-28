#include "DirectDamageSpell.h"
#include "game/Game.h"
#include <iostream>
#include <string>
#include <cmath>

namespace Game {

DirectDamageSpell::DirectDamageSpell(int baseDamage, int baseRange)
    : baseDamage(baseDamage), baseRange(baseRange) {}

std::string DirectDamageSpell::getName() const {
    return "Direct Damage";
}

std::string DirectDamageSpell::getDescription() const {
    return "Deals damage to a single enemy or enemy structure in range.";
}

bool DirectDamageSpell::requiresTarget() const { return true; }

bool DirectDamageSpell::cast(Game& game, const Utils::Point& target, int enhancementStacks) {
    Player& player = game.getPlayer();
    int currentRange = baseRange + enhancementStacks;
    int currentDamage = baseDamage; 

    int distance = std::abs(player.getPosition().row - target.row) + std::abs(player.getPosition().column - target.column);

    if (distance > currentRange) {
        std::cout << "Target is out of range." << std::endl;
        return false;
    }

    if (auto* enemy = game.getEnemyAtPosition(target)) {
        enemy->takeDamage(currentDamage);
        std::cout << "You dealt " << currentDamage << " damage to an enemy at (" << target.row << ", " << target.column << ")." << std::endl;
        
        if (!enemy->isAlive()) {
            std::cout << "Enemy defeated by a spell!" << std::endl;
            player.addScore(100);
            player.onEnemyKilled();
        }
        return true;
    }
    if (auto* building = game.getBuildingAtPosition(target)) {
        building->takeDamage(currentDamage);
        std::cout << "You dealt " << currentDamage << " damage to an enemy building at (" << target.row << ", " << target.column << ")." << std::endl;
        return true;
    }
    if (auto* tower = game.getTowerAtPosition(target)) {
        tower->takeDamage(currentDamage);
        std::cout << "You dealt " << currentDamage << " damage to an enemy tower at (" << target.row << ", " << target.column << ")." << std::endl;
        return true;
    }

    std::cout << "There is no valid target at the selected position." << std::endl;
    return false;
}

bool DirectDamageSpell::cast(Game& game, const Utils::Point& casterPosition, const Utils::Point& target) {
    int distance = std::abs(casterPosition.row - target.row) + std::abs(casterPosition.column - target.column);

    if (distance > baseRange) {
        return false;
    }

    if (game.getPlayer().getPosition() == target) {
        game.getPlayer().takeDamage(baseDamage);
        std::cout << "A direct spell hits the player for " << baseDamage << " damage!" << std::endl;
        return true;
    }
    if (auto* ally = game.getAllyAtPosition(target)) {
        ally->takeDamage(baseDamage);
        std::cout << "A direct spell hits an ally for " << baseDamage << " damage!" << std::endl;
        return true;
    }

    return false;
}

SpellType DirectDamageSpell::getType() const {
    return SpellType::DirectDamage;
}

void DirectDamageSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&baseDamage), sizeof(baseDamage));
    os.write(reinterpret_cast<const char*>(&baseRange), sizeof(baseRange));
}

void DirectDamageSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&baseDamage), sizeof(baseDamage));
    is.read(reinterpret_cast<char*>(&baseRange), sizeof(baseRange));
}

}