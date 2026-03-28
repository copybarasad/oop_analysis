#include "SpellSystem.h"
#include "GameController.h" 
#include "Hand.h"           
#include "Player.h"         
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"
#include <iostream>
#include <cmath>
#include <string>

bool SpellSystem::castSpell(ISpell &spell, GameController &controller, const Position &casterPosition, const Position &targetPos) {
    return spell.apply(controller, casterPosition, targetPos);
}

bool SpellSystem::validateSpellTarget(const Position &casterPos, const Position &targetPos, int maxRange) {
    int dist = std::abs(casterPos.x - targetPos.x) + std::abs(casterPos.y - targetPos.y);
    return dist <= maxRange;
}
bool SpellSystem::upgradeSpell(Hand &hand, int spellIndex, Player &player) {
    if (spellIndex < 0 || spellIndex >= hand.size()) {
        std::cout << "Invalid spell index!" << std::endl;
        return false;
    }
    
    const ISpell* spell = hand.getAt(spellIndex);
    if (!spell) {
        std::cout << "Spell not found!" << std::endl;
        return false;
    }
    
    if (!canSpellBeUpgraded(spell)) {
        std::cout << "This spell cannot be upgraded!" << std::endl;
        return false;
    }
    
    int upgradeCost = 25;
    if (!player.trySpendCoins(upgradeCost)) {
        std::cout << "Not enough coins for upgrade! Need " << upgradeCost << " coins." << std::endl;
        return false;
    }
    
    bool success = hand.upgradeSpell(spellIndex, 1);
    
    if (success) {
        std::cout << "Spell upgraded successfully! (-" << upgradeCost << " coins)" << std::endl;
        const ISpell* upgradedSpell = hand.getAt(spellIndex);
        std::cout << "Upgraded: " << upgradedSpell->name() << std::endl;
        std::cout << getSpellUpgradeInfo(upgradedSpell) << std::endl;
    } else {
        std::cout << "Upgrade failed!" << std::endl;
        player.addCoins(upgradeCost);
    }
    
    return success;
}

bool SpellSystem::canSpellBeUpgraded(const ISpell* spell) {
    if (!spell) return false;
    
    std::string spellName = spell->name();
    return spellName == "Direct Damage" || 
           spellName == "Area Damage" || 
           spellName == "Trap" || 
           spellName == "Summon";
}

std::string SpellSystem::getSpellUpgradeInfo(const ISpell* spell) {
    if (!spell) return "Unknown spell";
    
    std::string spellName = spell->name();
    
    if (spellName == "Direct Damage") {
        const DirectDamageSpell* directSpell = dynamic_cast<const DirectDamageSpell*>(spell);
        if (directSpell) {
            return "Direct Damage - improved damage and range";
        }
    }
    else if (spellName == "Area Damage") {
        return "Area Damage - improved damage and area size";
    }
    else if (spellName == "Trap") {
        return "Trap - improved damage";
    }
    else if (spellName == "Summon") {
        return "Summon - summons more allies";
    }
    else if (spellName == "Upgrade") {
        return "Upgrade - increased upgrade power";
    }
    
    return "No upgrade information available";
}