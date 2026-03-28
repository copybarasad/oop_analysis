#include "LevelUpSystem.h"

LevelUpSystem::LevelUpSystem(Player& player) 
    : player(player) {
}

void LevelUpSystem::performLevelUp(int completedLevel) {
    std::cout << "You have completed level " << completedLevel << "!" << std::endl;
    
    showLevelUpMenu();
    Upgrade choice = getPlayerUpgradeChoice();
    applyUpgrade(choice);
}

void LevelUpSystem::showLevelUpMenu() const {
    std::cout << "==================" << std::endl;
    std::cout << "Choose one improvement:" << std::endl;
    std::cout << "1. +25 Max Health (Current Max: " << player.getMaxHealthPoints() << " HP)" << std::endl;  // Используем метод из Unit
    std::cout << "2. +8 Damage (Current: " << player.getDamage() << ")" << std::endl;
    std::cout << "3. +1 Spell Capacity (Current: " 
    << player.getHand().getMaxHandSize() << " slots)" << std::endl;
    std::cout << "==================" << std::endl;
}

LevelUpSystem::Upgrade LevelUpSystem::getPlayerUpgradeChoice() const {
    int choice = 0;
    while (true) {
        std::cout << "Enter your choice (1-3): ";
        std::cin >> choice;        
        switch (choice) {
            case 1: return Upgrade::HEALTH;
            case 2: return Upgrade::DAMAGE;
            case 3: return Upgrade::SPELL_CAPACITY;
            default:
                std::cout << "Invalid choice! Please enter 1-3." << std::endl;
        }
    }
}

void LevelUpSystem::applyUpgrade(Upgrade upgrade) {
    switch (upgrade) {
        case Upgrade::HEALTH: {
            int newMaxHealth = player.getMaxHealthPoints() + 25;
            player.setMaxHealth(newMaxHealth);
            std::cout << "Max health increased to " << newMaxHealth << " HP!" << std::endl;
            break;
        }
        case Upgrade::DAMAGE: {
            player.setDamageBonus(player.getDamageBonus() + 8);
            std::cout << "Damage increased to " << player.getDamage() << "!" << std::endl;
            break;
        }
        case Upgrade::SPELL_CAPACITY: {
            auto& hand = player.getHand();
            hand.increaseMaxHandSize(1);
            std::cout << "Spell capacity increased to " << hand.getMaxHandSize() << " slots!" << std::endl;
            break;
        }
    }
}