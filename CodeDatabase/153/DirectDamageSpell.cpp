#include "DirectDamageSpell.h"
#include "GameField.h"
#include "Enemy.h"
#include "SpellTarget.h"
#include "Player.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string& spellName, int cost, int spellRange, int spellDamage)
    : SpellBase(spellName, cost, spellRange, spellDamage) {
}

bool DirectDamageSpell::cast(const SpellTarget& target, GameField& field,
    std::vector<Enemy>& enemies, Player& player) {
    if (!validateTarget(target, field)) {
        return false;
    }

    bool enemyFound = false;
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == target.targetX && enemy.getY() == target.targetY) {
            enemy.takeDamage(damage);
            std::cout << name << " deals " << damage << " damage to enemy!" << std::endl;

            if (!enemy.isAlive()) {
                std::cout << "Enemy defeated!" << std::endl;
                player.addScore(10);
            }

            enemyFound = true;
            break;
        }
    }

    if (!enemyFound) {
        std::cout << "No enemy on target cell! Spell failed." << std::endl;
        return false;
    }

    return true;
}

std::string DirectDamageSpell::getDescription() const {
    return name + " - damage: " + std::to_string(damage) + ", range: " + std::to_string(range);
}

std::unique_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(name, manaCost, range, damage);
}