#include "upgrade_system.h"
#include "game.h"
#include "game_input_handler.h"
#include "renderer.h"
#include <iostream>
#include <iomanip>

void UpgradeSystem::displayUpgradeMenu(Game& game) {
    while (true) {
        displayUpgradeInfo(game);
        
        std::cout << "\n=== UPGRADE MENU ===\n";
        std::cout << "Current Score: " << game.getPlayerScore() << " points\n\n";
        std::cout << "1 - Upgrade Max HP (+10, costs 50 points)\n";
        std::cout << "2 - Upgrade Melee Damage (+1, costs 25 points)\n";
        std::cout << "3 - Upgrade Ranged Damage (+1, costs 20 points)\n";
        std::cout << "0 - Finish upgrades and start level\n";
        std::cout << "\nSelect option: ";
        
        char choice;
        std::cin >> choice;
        std::cin.ignore(10000, '\n');
        
        bool upgraded = false;
        
        switch (choice) {
            case '1':
                upgraded = upgradeMaxHealth(game, 50);
                if (upgraded) {
                    std::cout << "+ Max HP increased! New Max HP: " << game.player.getMaxHealth() << "\n";
                } else {
                    std::cout << "- Not enough points for this upgrade!\n";
                }
                break;
                
            case '2':
                upgraded = upgradeMeleeDamage(game, 25);
                if (upgraded) {
                    std::cout << "+ Melee Damage increased! New Melee DMG: " << game.player.getMeleeDamage() << "\n";
                } else {
                    std::cout << "- Not enough points for this upgrade!\n";
                }
                break;
                
            case '3':
                upgraded = upgradeRangedDamage(game, 20);
                if (upgraded) {
                    std::cout << "+ Ranged Damage increased! New Ranged DMG: " << game.player.getRangedDamage() << "\n";
                } else {
                    std::cout << "- Not enough points for this upgrade!\n";
                }
                break;
                
            case '0':
                std::cout << "Starting next level...\n\n";
                return;
                
            default:
                std::cout << "Invalid option!\n";
                break;
        }
    }
}

void UpgradeSystem::displayUpgradeInfo(const Game& game) {
    std::cout << "\n========== PLAYER STATS ==========\n";
    std::cout << "Health:        " << std::setw(3) << game.player.getHealth() 
              << " / " << std::setw(3) << game.player.getMaxHealth() << "\n";
    std::cout << "Melee Damage:  " << std::setw(3) << game.player.getMeleeDamage() << "\n";
    std::cout << "Ranged Damage: " << std::setw(3) << game.player.getRangedDamage() << "\n";
    std::cout << "Score:         " << std::setw(3) << game.getPlayerScore() << "\n";
    std::cout << "==================================\n";
}

bool UpgradeSystem::upgradeMaxHealth(Game& game, int costPerHP) {
    const int HP_INCREASE = 10;

    if (game.getPlayerScore() < costPerHP) {
        return false;
    }

    game.player.increaseScore(-costPerHP);

    int newMaxHealth = game.player.getMaxHealth() + HP_INCREASE;
    game.player.setMaxHealth(newMaxHealth);
    game.player.setHealth(newMaxHealth);

    return true;
}

bool UpgradeSystem::upgradeMeleeDamage(Game& game, int costPerDamage) {
    const int DAMAGE_INCREASE = 1;
    
    if (game.getPlayerScore() < costPerDamage) {
        return false;
    }
    
    game.player.increaseScore(-costPerDamage);
    game.player.increaseMeleeDamage(DAMAGE_INCREASE);
    
    return true;
}

bool UpgradeSystem::upgradeRangedDamage(Game& game, int costPerDamage) {
    const int DAMAGE_INCREASE = 1;
    
    if (game.getPlayerScore() < costPerDamage) {
        return false;
    }
    
    game.player.increaseScore(-costPerDamage);
    game.player.increaseRangedDamage(DAMAGE_INCREASE);
    
    return true;
}