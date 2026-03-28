#include "AreaDamageSpell.h"
#include "GameField.h"
#include "SpellTarget.h"
#include "Enemy.h"
#include "Player.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(const std::string& spellName, int cost, int spellRange, int spellDamage)
    : SpellBase(spellName, cost, spellRange, spellDamage) {
}

bool AreaDamageSpell::cast(const SpellTarget& target, GameField& field,
    std::vector<Enemy>& enemies, Player& player) {
    if (!validateTarget(target, field)) {
        return false;
    }

    std::cout << name << " hits 2x2 area!" << std::endl;
    int enemiesHit = 0;

    for (int y = target.targetY; y <= target.targetY + 1; y++) {
        for (int x = target.targetX; x <= target.targetX + 1; x++) {
            if (field.isValidPosition(x, y)) {
                for (auto& enemy : enemies) {
                    if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
                        enemy.takeDamage(damage);
                        std::cout << "Enemy at (" << x << "," << y << ") takes "
                            << damage << " damage!" << std::endl;

                        if (!enemy.isAlive()) {
                            std::cout << "Enemy defeated!" << std::endl;
                            player.addScore(10);
                        }

                        enemiesHit++;
                    }
                }
            }
        }
    }

    if (enemiesHit == 0) {
        std::cout << "No enemies found in the affected area." << std::endl;
    }

    return true;
}

std::string AreaDamageSpell::getDescription() const {
    return name + " - area damage: " + std::to_string(damage) + " (2x2), range: " + std::to_string(range);
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(name, manaCost, range, damage);
}