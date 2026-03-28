#include "UpgradeSystem.h"
#include "Player.h"
#include <iostream>
#include <windows.h>

std::vector<UpgradeOption> UpgradeSystem::getAvailableUpgrades(std::shared_ptr<Player> player) {
    std::vector<UpgradeOption> upgrades;
    
    upgrades.push_back({
        "Увеличение урона", 
        "Увеличивает базовый урон на 5",
        upgradeDamage
    });
    
    upgrades.push_back({
        "Увеличение максимального здоровья",
        "Увеличивает максимальное здоровье на 20",
        upgradeMaxHealth
    });
    
    upgrades.push_back({
        "Увеличение вместимости заклинаний",
        "Увеличивает максимальное количество заклинаний на 1",
        upgradeSpellCapacity
    });
    
    upgrades.push_back({
        "Увеличение дальности атаки",
        "Увеличивает дальность дальней атаки на 1 клетку",
        upgradeRangedRange
    });
    
    upgrades.push_back({
        "Улучшение подвижности",
        "Уменьшает эффект замедления на 50%",
        upgradeMovement
    });
    
    return upgrades;
}

bool UpgradeSystem::applyUpgrade(std::shared_ptr<Player> player, const std::string& upgradeName) {
    auto upgrades = getAvailableUpgrades(player);
    
    for (const auto& upgrade : upgrades) {
        if (upgrade.name == upgradeName) {
            upgrade.apply(player);
            return true;
        }
    }
    
    return false;
}

void UpgradeSystem::displayUpgradeMenu(std::shared_ptr<Player> player) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    std::cout << "\n=== ВЫБОР УЛУЧШЕНИЯ ===" << std::endl;
    std::cout << "Выберите одно улучшение из списка:" << std::endl;
    
    auto upgrades = getAvailableUpgrades(player);
    for (size_t i = 0; i < upgrades.size(); ++i) {
        std::cout << i + 1 << ". " << upgrades[i].name << " - " << upgrades[i].description << std::endl;
    }
    
    std::cout << "Введите номер улучшения (1-" << upgrades.size() << "): ";
}

void UpgradeSystem::upgradeDamage(std::shared_ptr<Player> player) {
    player->increaseDamage(5);
}

void UpgradeSystem::upgradeMaxHealth(std::shared_ptr<Player> player) {
    int oldHp = player->getHp();
    int oldMaxHp = player->getMaxHp();

    player->increaseMaxHp(20);
    
    int healAmount = player->getMaxHp() - oldHp;
    if (healAmount > 0) {
        player->heal(healAmount);
    }
}

void UpgradeSystem::upgradeSpellCapacity(std::shared_ptr<Player> player) {
    player->getHand().increaseMaxSize(1);
}

void UpgradeSystem::upgradeRangedRange(std::shared_ptr<Player> player) {
    player->increaseRangedRange(1);
}

void UpgradeSystem::upgradeMovement(std::shared_ptr<Player> player) {
    player->increaseSlowResistance(0.5f);
}