#pragma once
#include "Player.h"
#include <vector>
#include <functional>
#include <string>

struct UpgradeOption {
    std::string name;
    std::string description;
    std::function<void(std::shared_ptr<Player>)> apply;
};

class UpgradeSystem {
public:
    static std::vector<UpgradeOption> getAvailableUpgrades(std::shared_ptr<Player> player);
    static bool applyUpgrade(std::shared_ptr<Player> player, const std::string& upgradeName);
    static void displayUpgradeMenu(std::shared_ptr<Player> player);
    
private:
    static void upgradeDamage(std::shared_ptr<Player> player);
    static void upgradeMaxHealth(std::shared_ptr<Player> player);
    static void upgradeSpellCapacity(std::shared_ptr<Player> player);
    static void upgradeRangedRange(std::shared_ptr<Player> player);
    static void upgradeMovement(std::shared_ptr<Player> player);
};