#include "UpgradeSystem.hpp"
#include <iostream>
#include <algorithm>
#include <random>

UpgradeSystem::UpgradeSystem() {
    initializeUpgrades();
}

void UpgradeSystem::initializeUpgrades() {
    availableUpgrades = {
        {
            "Увеличить здоровье",
            "+30 к максимальному здоровью",
            [](Hero& hero, SpellHand&) { 
                int newMaxHealth = hero.getMaxHealth() + 30;
                int currentHealth = hero.getHealth();
                hero = Hero(hero.getPositionX(), hero.getPositionY(), 
                           newMaxHealth, newMaxHealth, hero.getMeleeDamage(), 
                           hero.getRangedDamage(), hero.getMana(), hero.getMaxMana());
                hero.takeDamage(-30);
            },
            50
        },
        {
            "Увеличить ближний урон", 
            "+5 к урону ближней атаки",
            [](Hero& hero, SpellHand&) {
                hero.setMeleeDamage(hero.getMeleeDamage() + 5);
            },
            40
        },
        {
            "Увеличить дальний урон",
            "+3 к урону дальней атаки", 
            [](Hero& hero, SpellHand&) {
                hero.setRangedDamage(hero.getRangedDamage() + 3);
            },
            35
        },
        {
            "Увеличить ману",
            "+20 к максимальной мане",
            [](Hero& hero, SpellHand&) {
                hero.setMaxMana(hero.getMaxMana() + 20);
                hero.gainMana(20);
            },
            30
        },
        {
            "Увеличить дальность атаки",
            "+1 к дальности дальней атаки",
            [](Hero& hero, SpellHand&) {
                hero.setRangedRange(hero.getRangedRange() + 1);
            },
            45
        }
    };
}

void UpgradeSystem::showUpgradeMenu(Hero& hero, SpellHand& spellHand) {
    std::cout << "\n=== ВЫБОР УЛУЧШЕНИЯ ===\n";
    std::cout << "Доступные очки: " << hero.getScore() << "\n\n";
    
    auto upgrades = getRandomUpgrades(2);
    
    for (size_t i = 0; i < upgrades.size(); ++i) {
        std::cout << i + 1 << ". " << upgrades[i].name << "\n";
        std::cout << "   " << upgrades[i].description << "\n";
        std::cout << "   Стоимость: " << upgrades[i].cost << " очков\n\n";
    }
    
    std::cout << "4. Пропустить улучшение (сохранить очки)\n";
    std::cout << "\nВыберите улучшение (1-4): ";
    
    int choice;
    std::cin >> choice;
    
    if (choice >= 1 && choice <= static_cast<int>(upgrades.size())) {
        UpgradeOption& selectedUpgrade = upgrades[choice - 1];
        
        if (canAffordUpgrade(hero, selectedUpgrade)) {
            hero.setScore(hero.getScore() - selectedUpgrade.cost);
            selectedUpgrade.applyUpgrade(hero, spellHand);
            std::cout << "Улучшение применено: " << selectedUpgrade.name << "\n";
        } else {
            std::cout << "Недостаточно очков для этого улучшения!\n";
        }
    } else if (choice == 4) {
        std::cout << "Улучшения пропущены. Очки сохранены.\n";
    } else {
        std::cout << "Неверный выбор. Улучшения пропущены.\n";
    }
    
    std::cout << "Нажмите любую клавишу для продолжения...";
    std::cin.ignore();
    std::cin.get();
}

std::vector<UpgradeOption> UpgradeSystem::getRandomUpgrades(int count) {
    std::vector<UpgradeOption> result;
    std::sample(availableUpgrades.begin(), availableUpgrades.end(),
                std::back_inserter(result), count,
                std::mt19937{std::random_device{}()});
    return result;
}

bool UpgradeSystem::canAffordUpgrade(const Hero& hero, const UpgradeOption& upgrade) const {
    return hero.getScore() >= upgrade.cost;
}