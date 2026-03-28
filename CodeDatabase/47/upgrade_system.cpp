#include "upgrade_system.h"
#include <iostream>

void UpgradeSystem::showUpgradeMenu(Player& player) {
    std::cout << "\n=== UPGRADE MENU ===" << std::endl;
    std::cout << "Current stats:" << std::endl;
    std::cout << "Health: " << player.getHealth() << "/" << player.getMaxHealth() << std::endl;
    std::cout << "Melee Damage: " << player.getMeleeDamage() << std::endl;
    std::cout << "Ranged Damage: " << player.getRangedDamage() << std::endl;
    std::cout << "Mana: " << player.getSpellHand().getCurrentMana() << "/" << player.getSpellHand().getMaxMana() << std::endl;

    std::cout << "Current Spells:" << std::endl;
    for (int i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
        auto spell = player.getSpellHand().getSpells()[i].get();
        std::cout << spell->getName() << " - Damage: " << spell->getDamage() << std::endl;
    }

    std::cout << "\nChoose an upgrade:" << std::endl;
    std::cout << "1. Increase Health (+" << HEALTH_BOOST << " HP)" << std::endl;
    std::cout << "2. Increase Melee Damage (+" << DAMAGE_BOOST << " damage)" << std::endl;
    std::cout << "3. Increase Ranged Damage (+" << DAMAGE_BOOST << " damage)" << std::endl;
    std::cout << "4. Increase Mana Capacity (+" << MANA_BOOST << " mana)" << std::endl;
    std::cout << "5. Enhance Spell Power" << std::endl;
    
    int choice;
    bool validChoice = false;
    
    while (!validChoice) {
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input! Please enter a number between 1 and 5." << std::endl;
            continue;
        }
        
        if (choice >= 1 && choice <= 5) {
            validChoice = true;
        }
        else {
            std::cout << "Invalid choice! Please enter a number between 1 and 5." << std::endl;
        }
    }
    
    switch (choice) {
        case 1:
            player.increaseMaxHealth(HEALTH_BOOST);
            std::cout << "Health increased by " << HEALTH_BOOST << "! Now: " << player.getMaxHealth() << std::endl;
            break;
        case 2:
            player.increaseMeleeDamage(DAMAGE_BOOST);
            std::cout << "Melee damage increased by " << DAMAGE_BOOST << "! Now: " << player.getMeleeDamage() << std::endl;
            break;
        case 3:
            player.increaseRangedDamage(DAMAGE_BOOST);
            std::cout << "Ranged damage increased by " << DAMAGE_BOOST << "! Now: " << player.getRangedDamage() << std::endl;
            break;
        case 4:
            player.increaseManaCapacity(MANA_BOOST);
            std::cout << "Mana capacity increased by " << MANA_BOOST << "! Now: " << player.getSpellHand().getMaxMana() << std::endl;
            break;
        case 5:
            player.enhanceSpellPower();
            std::cout << "Spell power enhanced! Half random spells in your next level deal +1 damage." << std::endl;
            break;
    }
}

void UpgradeSystem::removeRandomSpells(Player& player) {
    auto& spellHand = player.getSpellHand();
    int spellCount = spellHand.getSpellCount();
    
    if (spellCount > 1) {
        int spellsToRemove = spellCount / 2;
        std::cout << "Removing " << spellsToRemove << " random spells..." << std::endl;
        spellHand.removeRandomSpells(spellsToRemove);
    }
}