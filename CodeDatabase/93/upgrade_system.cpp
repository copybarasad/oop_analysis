#include "upgrade_system.h"
#include "input_manager.h"
#include <iostream>
#include <random>

void UpgradeSystem::showUpgradeMenu(Player& player) {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "LEVEL UP! Choose 1 UPGRADE\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "Current Level: " << player.getLevel() << "\n";
    std::cout << "Score: " << player.getScore() << "\n";
    std::cout << "\nAvailable Upgrades:\n";
    std::cout << "1. +20 Max Health (current: " << player.getMaxHealth() << ")\n";
    std::cout << "2. +5 Melee Damage (current: " << player.getMeleeDamage() << ")\n";
    std::cout << "3. +3 Ranged Damage (current: " << player.getRangedDamage() << ")\n";
    std::cout << "4. +1 Hand Size (current: " << player.getHand().getMaxSize() << ")\n";
    
    const auto& hand = player.getHand();
    if (hand.size() > 0) {
        std::cout << "5. Improve a Spell\n";
    } else {
        std::cout << "5. Improve a Spell (get spells first!)\n";
    }

    char choice = '\0';
    bool validChoice = false;
    while (!validChoice) {
        std::cout << "\nChoose (1-5): ";
        choice = InputManager::getInput();
        if (choice >= '1' && choice <= '5') {
            validChoice = true;
        } else {
            std::cout << "Invalid choice! Please enter 1-5.\n";
        }
    }

    std::cout << "\n";

    switch (choice) {
        case '1':
            player.upgradeMaxHealth(20);
            std::cout << "Health increased to " << player.getMaxHealth() << "!\n";
            break;
        case '2':
            player.upgradeMeleeDamage(5);
            std::cout << "Melee damage increased to " << player.getMeleeDamage() << "!\n";
            break;
        case '3':
            player.upgradeRangedDamage(3);
            std::cout << "Ranged damage increased to " << player.getRangedDamage() << "!\n";
            break;
        case '4':
            if (player.getHand().getMaxSize() < 5) {
                player.getHand().upgradeMaxSize(1);
                std::cout << "Hand size increased to " << player.getHand().getMaxSize() << "!\n";
            } else {
                std::cout << "Hand size is already at maximum (5)!\n";
            }
            break;
        case '5':
            if (hand.size() == 0) {
                std::cout << "You have no spells to upgrade!\n";
                break;
            }
            std::cout << "Which spell to upgrade?\n";
            for (size_t i = 0; i < hand.size(); ++i) {
                std::cout << "  [" << (i+1) << "] " << hand.getSpells()[i]->getName()
                          << " — " << hand.getSpells()[i]->getDescription() << "\n";
            }
            std::cout << "Enter number: ";
            int idx;
            if (std::cin >> idx && idx >= 1 && idx <= (int)hand.size()) {
                std::cin.ignore();
                hand.getSpells()[idx-1]->upgrade();
                std::cout << "Spell upgraded!\n";
            } else {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "Invalid number.\n";
            }
            break;
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}