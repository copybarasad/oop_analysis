#include "SpellEffectSystem.h"
#include "Player.h"
#include "GameObject.h"
#include <iostream>

void SpellEffectSystem::applyDamageWithReward(Player& caster, GameObject& target, int damage, const std::string& spellName, const std::string& targetType) {
    if (!target.isAlive()) {
        std::cout << spellName << " skipped - target already dead!" << std::endl;
        return;
    }

    int healthBefore = target.getHealth();
    target.takeDamage(damage);
    int healthAfter = target.getHealth();

    std::cout << "*** " << spellName << " hits " << targetType << " at ("
        << target.getPositionX() << "," << target.getPositionY()
        << ") for " << damage << " damage! ";
    std::cout << "Health: " << healthBefore << " -> " << healthAfter;

    if (!target.isAlive()) {
        std::cout << " - TARGET ELIMINATED!" << std::endl;
        grantKillReward(caster, targetType);
    }
    else {
        std::cout << std::endl;
    }
}

void SpellEffectSystem::applyDamageToTarget(GameObject& target, int damage, const std::string& spellName, const std::string& targetType) {
    if (!target.isAlive()) {
        std::cout << spellName << " skipped - target already dead!" << std::endl;
        return;
    }

    int healthBefore = target.getHealth();
    target.takeDamage(damage);
    int healthAfter = target.getHealth();

    std::cout << "*** " << spellName << " hits " << targetType << " at ("
        << target.getPositionX() << "," << target.getPositionY()
        << ") for " << damage << " damage! ";
    std::cout << "Health: " << healthBefore << " -> " << healthAfter;

    if (!target.isAlive()) {
        std::cout << " - TARGET ELIMINATED!" << std::endl;
    }
    else {
        std::cout << std::endl;
    }
}

void SpellEffectSystem::grantKillReward(Player& caster, const std::string& targetType) {
    if (targetType == "enemy") {
        caster.addScore(100);
        caster.addMana(25);
        std::cout << "*** Enemy defeated! +100 score, +25 mana" << std::endl;
    }
    else if (targetType == "tower") {
        caster.addScore(200);
        caster.addMana(50);
        std::cout << "*** Tower destroyed! +200 score, +50 mana" << std::endl;
    }
}