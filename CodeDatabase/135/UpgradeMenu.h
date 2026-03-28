#ifndef UPGRADE_MENU_H
#define UPGRADE_MENU_H

#include "UpgradeSystem.h"
#include "Player.h"
#include <vector>

class UpgradeMenu {
public:
	UpgradeMenu() = default;
	
	void showUpgradeScreen(Player& player, UpgradeSystem& upgrade_system);

private:
	void displayUpgradeOptions(const Player& player);
	UpgradeType selectUpgrade();
	void clearInputBuffer();
	void displayPlayerStats(const Player& player);
};

#endif
