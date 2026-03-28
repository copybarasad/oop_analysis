#include "UpgradeSpell.h"
#include "GameController.h"
#include "Hand.h"
#include "SpellSystem.h"
#include "Player.h"
#include <iostream>

UpgradeSpell::UpgradeSpell(int p) : power(p) {}

bool UpgradeSpell::apply(GameController &controller, const Position &casterPosition, const Position &targetPos)
{
    Hand& hand = controller.getHand();
    Player& player = controller.getPlayerMutable();
    
    std::cout << "=== UPGRADE SPELL MENU ===" << std::endl;
    std::cout << "Upgrade power: " << power << std::endl;
    std::cout << "Select spell to upgrade (0 to cancel):" << std::endl;
    
    for (int i = 0; i < hand.size(); ++i) {
        const ISpell* spell = hand.getAt(i);
        std::cout << i << ") " << spell->name();
        if (SpellSystem::canSpellBeUpgraded(spell)) {
            std::cout << " [Upgradable]";
        }
        std::cout << std::endl;
    }
    std::cout << "-1) Cancel" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    if (choice == 0 || choice > hand.size()) {
        std::cout << "Upgrade cancelled." << std::endl;
        return false;
    }
    
    bool upgraded = SpellSystem::upgradeSpell(hand, choice, player);
    
    if (upgraded) {
        std::cout << "Spell successfully upgraded using Upgrade spell!" << std::endl;
        return true;
    } else {
        std::cout << "Upgrade failed!" << std::endl;
        return false;
    }
}