#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

#include "player.h"
#include <vector>
#include <string>

class UpgradeSystem {
public:
    UpgradeSystem();
    
    struct UpgradeOption {
        int id;
        std::string name;
        std::string description;
        int costPoints;
    };

    std::vector<UpgradeOption> getAvailableUpgrades() const;

    void applyUpgrade(int upgradeId, Player* player);

    void showUpgradeMenu(Player* player);
    
private:
    
    static constexpr int HEALTH_BOOST = 1;
    static constexpr int MELEE_DAMAGE_BOOST = 2;
    static constexpr int RANGED_DAMAGE_BOOST = 3;
    static constexpr int SPELL_SLOT = 4;
    static constexpr int SPELL_POWER = 5;
};

#endif
