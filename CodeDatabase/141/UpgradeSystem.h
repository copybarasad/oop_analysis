#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

#include "Player.h"
#include "Hand.h"
#include <vector>
#include <string>

enum class UpgradeType {
    INCREASE_HEALTH,
    INCREASE_MELEE_DAMAGE,
    INCREASE_RANGED_DAMAGE,
    UPGRADE_SPELL
};

struct UpgradeOption {
    UpgradeType type;
    std::string description;
    int value;
    int spellIndex;
};

class UpgradeSystem {
public:
    static std::vector<UpgradeOption> generateUpgradeOptions(const Player &player);

    static void applyUpgrade(Player &player, Hand &hand, const UpgradeOption &upgrade);

    static void displayUpgradeMenu(const std::vector<UpgradeOption> &options);
};

#endif
