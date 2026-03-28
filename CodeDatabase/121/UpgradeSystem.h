#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

#include "Player.h"
#include "TextSystem.h"
#include <memory>

enum class UpgradeType {
    HEALTH_BOOST,
    DAMAGE_BOOST, 
    SPELL_POWER
    // Убрали ATTACK_RANGE
};

class UpgradeSystem {
private:
    std::shared_ptr<TextSystem> textSystem;

public:
    UpgradeSystem(std::shared_ptr<TextSystem> textSystem);
    
    void showUpgradeMenu(Player& player);
    void applyUpgrade(Player& player, UpgradeType upgrade);
    
private:
    int getUpgradeCost(UpgradeType upgrade) const;
};

#endif