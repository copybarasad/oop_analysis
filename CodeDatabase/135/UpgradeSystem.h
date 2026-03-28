#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

#include "Player.h"

enum class UpgradeType {
	INTELLIGENCE,
	DEXTERITY,
	STRENGTH,
	MAX_HEALTH,
	HAND_SIZE,
	SPELL_ENHANCEMENT
};

class UpgradeSystem {
private:
	static const int ATTRIBUTE_UPGRADE_AMOUNT = 3;
	static const int HEALTH_UPGRADE_AMOUNT = 20;
	static const int HAND_SIZE_UPGRADE_AMOUNT = 1;

public:
	UpgradeSystem() = default;
	
	void applyUpgrade(Player& player, UpgradeType upgrade_type);
	
private:
	void upgradeIntelligence(Player& player);
	void upgradeDexterity(Player& player);
	void upgradeStrength(Player& player);
	void upgradeMaxHealth(Player& player);
	void upgradeHandSize(Player& player);
	void upgradeSpellEnhancement(Player& player);
};

#endif
