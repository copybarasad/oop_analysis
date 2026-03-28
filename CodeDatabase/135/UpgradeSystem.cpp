#include "UpgradeSystem.h"

void UpgradeSystem::applyUpgrade(Player& player, UpgradeType upgrade_type) {
	switch (upgrade_type) {
		case UpgradeType::INTELLIGENCE:
			upgradeIntelligence(player);
			break;
		case UpgradeType::DEXTERITY:
			upgradeDexterity(player);
			break;
		case UpgradeType::STRENGTH:
			upgradeStrength(player);
			break;
		case UpgradeType::MAX_HEALTH:
			upgradeMaxHealth(player);
			break;
		case UpgradeType::HAND_SIZE:
			upgradeHandSize(player);
			break;
		case UpgradeType::SPELL_ENHANCEMENT:
			upgradeSpellEnhancement(player);
			break;
	}
}

void UpgradeSystem::upgradeIntelligence(Player& player) {
	player.improveIntelligence(ATTRIBUTE_UPGRADE_AMOUNT);
}

void UpgradeSystem::upgradeDexterity(Player& player) {
	player.improveDexterity(ATTRIBUTE_UPGRADE_AMOUNT);
}

void UpgradeSystem::upgradeStrength(Player& player) {
	player.improveStrength(ATTRIBUTE_UPGRADE_AMOUNT);
}

void UpgradeSystem::upgradeMaxHealth(Player& player) {
	player.increaseMaxHealth(HEALTH_UPGRADE_AMOUNT);
}

void UpgradeSystem::upgradeHandSize(Player& player) {
	player.getSpellHand().increaseMaxSize(HAND_SIZE_UPGRADE_AMOUNT);
}

void UpgradeSystem::upgradeSpellEnhancement(Player& player) {
	player.getSpellEnhancer().addEnhancement();
}
