#include "PlayerUpgrade.h"
#include <iostream>
#include <algorithm>

// Конструктор системы улучшений
PlayerUpgrade::PlayerUpgrade() {
    // Инициализация пустого списка взятых улучшений
}

// Применение выбранного улучшения к игроку
void PlayerUpgrade::applyUpgrade(Player& player, UpgradeType type) {
    switch (type) {
    case UpgradeType::HEALTH_BOOST:
        player.setMaxHealth(player.getMaxHealth() + 20);
        player.restoreHealthToMax();
        std::cout << "Max health increased by 20! Current: " << player.getMaxHealth() << "\n";
        break;

    case UpgradeType::SPELL_POWER:
        increaseAllSpellDamage(player, 0.25f);
        std::cout << "Spell power increased! All spells deal 25% more damage.\n";
        break;

    case UpgradeType::SPELL_RANGE:
        increaseAllSpellRange(player, 1);
        markUpgradeTaken(UpgradeType::SPELL_RANGE);
        std::cout << "Spell range increased! All spells have +1 range.\n";
        std::cout << "This upgrade cannot be chosen again.\n";
        break;
    }
}

// Отображение меню выбора улучшений
void PlayerUpgrade::showUpgradeMenu(Player& player) {
    std::cout << "\n=== LEVEL UP - CHOOSE ONE UPGRADE ===\n";

    int optionNumber = 1;
    std::vector<UpgradeType> availableOptions;

    // Health Boost - всегда доступно
    std::cout << optionNumber << ". Health Boost (+20 max health, full heal)\n";
    availableOptions.push_back(UpgradeType::HEALTH_BOOST);
    optionNumber++;

    // Spell Power - всегда доступно
    std::cout << optionNumber << ". Spell Power (+25% damage to all spells)\n";
    availableOptions.push_back(UpgradeType::SPELL_POWER);
    optionNumber++;

    // Spell Range - доступно только если еще не брали
    if (isUpgradeAvailable(UpgradeType::SPELL_RANGE)) {
        std::cout << optionNumber << ". Spell Range (+1 range to all spells) [ONE-TIME ONLY]\n";
        availableOptions.push_back(UpgradeType::SPELL_RANGE);
        optionNumber++;
    }

    int choice;
    std::cout << "Enter your choice (1-" << (optionNumber - 1) << "): ";
    std::cin >> choice;
    std::cin.ignore();

    if (choice >= 1 && choice <= static_cast<int>(availableOptions.size())) {
        applyUpgrade(player, availableOptions[choice - 1]);
    }
    else {
        std::cout << "Invalid choice. Health boost applied by default.\n";
        applyUpgrade(player, UpgradeType::HEALTH_BOOST);
    }
}

// Проверка доступности улучшения
bool PlayerUpgrade::isUpgradeAvailable(UpgradeType type) const {
    // Spell Range можно взять только один раз
    if (type == UpgradeType::SPELL_RANGE) {
        return std::find(takenUpgrades_.begin(), takenUpgrades_.end(), type) == takenUpgrades_.end();
    }
    return true;
}

// Отметка улучшения как взятого
void PlayerUpgrade::markUpgradeTaken(UpgradeType type) {
    if (std::find(takenUpgrades_.begin(), takenUpgrades_.end(), type) == takenUpgrades_.end()) {
        takenUpgrades_.push_back(type);
    }
}

// Увеличение урона всех заклинаний игрока
void PlayerUpgrade::increaseAllSpellDamage(Player& player, float multiplier) {
    PlayerHand& hand = player.getHand();
    for (size_t i = 0; i < hand.getSpellCount(); ++i) {
        SpellCard* spell = hand.getSpell(i);
        if (spell) {
            int currentDamage = spell->getDamage();
            int increase = static_cast<int>(currentDamage * multiplier);
            spell->increaseDamage(increase);
        }
    }
}

// Увеличение радиуса всех заклинаний игрока
void PlayerUpgrade::increaseAllSpellRange(Player& player, int amount) {
    PlayerHand& hand = player.getHand();
    for (size_t i = 0; i < hand.getSpellCount(); ++i) {
        SpellCard* spell = hand.getSpell(i);
        if (spell) {
            spell->increaseRange(amount);
        }
    }
}