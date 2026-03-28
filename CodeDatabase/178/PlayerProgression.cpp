#include "PlayerProgression.h"
#include "GameConfig.h"
#include "SpellSystem.h"
#include <iostream>
#include <random>
#include <algorithm>

void PlayerProgression::applyLevelUp(Player& player, Hand& hand) {
    removeRandomSpells(hand, GameConfig::getSpellRemovePercentage());
    
    player.restoreHealth();
    
    std::cout << "\n=== LEVEL UP! ===" << std::endl;
    std::cout << "Half of your spells have been removed randomly." << std::endl;
    std::cout << "Your health has been restored." << std::endl;
    std::cout << "You can now upgrade your character!" << std::endl;
}

void PlayerProgression::showUpgradeMenu(Player& player) {
    std::cout << "\n=== UPGRADE MENU ===" << std::endl;
    std::cout << "Coins: " << player.getCoins() << std::endl;
    std::cout << "Upgrade cost: " << GameConfig::getUpgradeCost() << " coins" << std::endl;
    std::cout << "1. Upgrade Health (+20 HP)" << std::endl;
    std::cout << "2. Upgrade Melee Damage (+5)" << std::endl;
    std::cout << "3. Upgrade Ranged Damage (+3)" << std::endl;
    std::cout << "4. Skip upgrade" << std::endl;
    std::cout << "Choose an option (1-4): ";
}

bool PlayerProgression::upgradePlayerStat(Player& player, const std::string& statType, int cost) {
    if (!player.trySpendCoins(cost)) {
        std::cout << "Not enough coins for upgrade! Need " << cost << " coins." << std::endl;
        return false;
    }
    
    if (statType == "health") {
        player.upgradeHealth(20);
        std::cout << "Health upgraded! +20 HP" << std::endl;
        return true;
    }
    else if (statType == "melee") {
        player.upgradeMeleeDamage(5);
        std::cout << "Melee damage upgraded! +5 damage" << std::endl;
        return true;
    }
    else if (statType == "ranged") {
        player.upgradeRangedDamage(3);
        std::cout << "Ranged damage upgraded! +3 damage" << std::endl;
        return true;
    }
    
    return false;
}

bool PlayerProgression::upgradeSpell(Hand& hand, int spellIndex, Player& player) {
    return SpellSystem::upgradeSpell(hand, spellIndex, player);
}

void PlayerProgression::removeRandomSpells(Hand& hand, int percentage) {
    int currentSize = hand.size();
    if (currentSize <= 0) return;
    
    int spellsToRemove = (currentSize * percentage) / 100;
    if (spellsToRemove <= 0) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int i = 0; i < spellsToRemove && hand.size() > 0; i++) {
        std::uniform_int_distribution<> dis(0, hand.size() - 1);
        int indexToRemove = dis(gen);
        hand.removeAt(indexToRemove);
    }
}