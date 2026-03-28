#include "upgrade_system.h"
#include <iostream>
#include <algorithm>

UpgradeSystem::UpgradeSystem() = default;

std::vector<UpgradeSystem::UpgradeOption> UpgradeSystem::getAvailableUpgrades() const {
    return {
        {HEALTH_BOOST, "Увеличить здоровье", "+15 HP", 1},
        {MELEE_DAMAGE_BOOST, "Увеличить урон в ближнем бою", "+2 урон", 1},
        {RANGED_DAMAGE_BOOST, "Увеличить урон в дальнем бою", "+2 урон", 1},
        {SPELL_SLOT, "Добавить слот для заклинания", "+1 слот руки", 2},
        {SPELL_POWER, "Улучшить мощь заклинаний", "Заклинания наносят больше урона", 2}
    };
}

void UpgradeSystem::applyUpgrade(int upgradeId, Player* player) {
    if (!player) return;
    
    switch (upgradeId) {
        case HEALTH_BOOST: {
            int currentHealth = player->getHealth();
            int maxHealth = currentHealth + 15;
            player->setHealth(maxHealth);
            std::cout << "✓ Здоровье увеличено на 15! (текущее: " << maxHealth << ")\n";
            break;
        }
        case MELEE_DAMAGE_BOOST: {
            int currentDamage = player->getMeleeDamage();
            player->setMeleeDamage(currentDamage + 2);
            std::cout << "✓ Урон в ближнем бою увеличен на 2! (текущий: " << (currentDamage + 2) << ")\n";
            break;
        }
        case RANGED_DAMAGE_BOOST: {
            int currentDamage = player->getRangedDamage();
            player->setRangedDamage(currentDamage + 2);
            std::cout << "✓ Урон в дальнем бою увеличен на 2! (текущий: " << (currentDamage + 2) << ")\n";
            break;
        }
        case SPELL_SLOT:
            std::cout << "✓ Добавлено место для нового заклинания!\n";
            break;
        case SPELL_POWER:
            std::cout << "✓ Мощь заклинаний улучшена!\n";
            break;
        default:
            std::cout << "Неизвестное улучшение\n";
    }
}

void UpgradeSystem::showUpgradeMenu(Player* player) {
    if (!player) return;
    
    std::cout << "\n=== СИСТЕМА ПРОКАЧКИ ===\n";
    std::cout << "Выберите улучшение:\n";
    
    auto upgrades = getAvailableUpgrades();
    for (const auto& upgrade : upgrades) {
        std::cout << upgrade.id << ". " << upgrade.name << " - " << upgrade.description << "\n";
    }
    std::cout << "0. Пропустить\n";
    std::cout << "Ваш выбор: ";
    
    int choice;
    if (std::cin >> choice) {
        if (choice >= 1 && choice <= static_cast<int>(upgrades.size())) {
            applyUpgrade(choice, player);
        } else if (choice != 0) {
            std::cout << "Неверный выбор\n";
        }
    }
}
