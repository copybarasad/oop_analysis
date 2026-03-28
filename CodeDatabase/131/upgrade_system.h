#ifndef OOP_UPGRADE_SYSTEM_H_
#define OOP_UPGRADE_SYSTEM_H_

#include "player.h"
#include <string>
#include <vector>

namespace rpg {

struct Upgrade {
    UpgradeType type;
    std::string name;
    std::string description;
    int value;
};

class UpgradeSystem final {
public:
    UpgradeSystem();
    
    [[nodiscard]] std::vector<Upgrade> GetAvailableUpgrades(int level) const;
    
    void ApplyUpgrade(Player& player, const Upgrade& upgrade) const;
    
    void ApplyLevelTransition(Player& player) const;
    
private:
    [[nodiscard]] Upgrade CreateHealthUpgrade(int level) const;
    [[nodiscard]] Upgrade CreateMeleeDamageUpgrade(int level) const;
    [[nodiscard]] Upgrade CreateRangedDamageUpgrade(int level) const;
};

}

#endif


