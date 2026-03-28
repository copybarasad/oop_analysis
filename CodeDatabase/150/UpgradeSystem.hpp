#ifndef UPGRADESYSTEM_H
#define UPGRADESYSTEM_H

#include "Hero.hpp"
#include "SpellHand.hpp"
#include <vector>
#include <functional>
#include <string>
#include <random>

struct UpgradeOption {
    std::string name;
    std::string description;
    std::function<void(Hero&, SpellHand&)> applyUpgrade;
    int cost;
};

class UpgradeSystem {
private:
    std::vector<UpgradeOption> availableUpgrades;
    void initializeUpgrades();
    
public:
    UpgradeSystem();
    void showUpgradeMenu(Hero& hero, SpellHand& spellHand);
    std::vector<UpgradeOption> getRandomUpgrades(int count);
    bool canAffordUpgrade(const Hero& hero, const UpgradeOption& upgrade) const;
};

#endif