#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

#include <string>

class Game;

class UpgradeSystem {
public:
    static void displayUpgradeMenu(Game& game);
    static bool upgradeMaxHealth(Game& game, int costPerHP);
    static bool upgradeMeleeDamage(Game& game, int costPerDamage);
    static bool upgradeRangedDamage(Game& game, int costPerDamage);

private:
    static void displayUpgradeInfo(const Game& game);
    static int getUpgradeCost(int costPerPoint);
};

#endif