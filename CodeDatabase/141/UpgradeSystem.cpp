#include "UpgradeSystem.h"
#include "Player.h"
#include "Hand.h"
#include "Spell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "GameConstants.h"
#include <iostream>
#include <algorithm>
#include <unordered_set>

std::vector<UpgradeOption> UpgradeSystem::generateUpgradeOptions(const Player &player) {
    std::vector<UpgradeOption> options;

    options.push_back({
        UpgradeType::INCREASE_HEALTH,
        "Увеличить запас здоровья на 20", 20, -1
    });

    options.push_back({
        UpgradeType::INCREASE_MELEE_DAMAGE,
        "Увеличить урон ближнего боя на 5", 5, -1
    });

    options.push_back({
        UpgradeType::INCREASE_RANGED_DAMAGE,
        "Увеличить урон дальнего боя на 5", 5, -1
    });

    const auto &hand = player.getHand();
    std::unordered_set<std::string> seenSpells;
    for (int i = 0; i < hand.getSize(); ++i) {
        Spell *spell = hand.getSpell(i);
        if (spell) {
            std::string spellName = spell->getName();
            if (seenSpells.insert(spellName).second) {
                options.push_back({
                    UpgradeType::UPGRADE_SPELL,
                    "Улучшить " + spellName, 0, i
                });
            }
        }
    }

    return options;
}

void UpgradeSystem::applyUpgrade(Player &player, Hand &hand, const UpgradeOption &upgrade) {
    switch (upgrade.type) {
        case UpgradeType::INCREASE_HEALTH: {
            int currentHealth = player.getHealth();
            int maxHealth = GameConstants::PLAYER_INITIAL_HEALTH;
            player.setHealth(std::min(currentHealth + upgrade.value, maxHealth + upgrade.value));
            break;
        }
        case UpgradeType::INCREASE_MELEE_DAMAGE: {
            player.setMeleeDamage(player.getMeleeDamage() + upgrade.value);
            break;
        }
        case UpgradeType::INCREASE_RANGED_DAMAGE: {
            player.setRangedDamage(player.getRangedDamage() + upgrade.value);
            break;
        }
        case UpgradeType::UPGRADE_SPELL: {
            if (upgrade.spellIndex >= 0 && upgrade.spellIndex < hand.getSize()) {
                Spell *spell = hand.getSpell(upgrade.spellIndex);
                if (spell) {
                    Spell *upgraded = spell->upgradedCopy();
                    if (upgraded) {
                        hand.removeSpell(upgrade.spellIndex);
                        if (!hand.addSpell(upgraded)) {
                            delete upgraded;
                        }
                    }
                }
            }
            break;
        }
    }
}

void UpgradeSystem::displayUpgradeMenu(const std::vector<UpgradeOption> &options) {
    std::cout << "\n=== UPGRADE MENU ===\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << (i + 1) << ". " << options[i].description << "\n";
    }
    std::cout << "Choose an upgrade (1-" << options.size() << "): ";
}
