#include "upgrade_system.h"
#include <random>
#include <algorithm>

namespace rpg {

UpgradeSystem::UpgradeSystem() {}

std::vector<Upgrade> UpgradeSystem::GetAvailableUpgrades(int level) const {
    std::vector<Upgrade> upgrades;
    
    upgrades.push_back(CreateHealthUpgrade(level));
    upgrades.push_back(CreateMeleeDamageUpgrade(level));
    upgrades.push_back(CreateRangedDamageUpgrade(level));
    
    return upgrades;
}

void UpgradeSystem::ApplyUpgrade(Player& player, const Upgrade& upgrade) const {
    player.ApplyUpgrade(upgrade.type, upgrade.value);
}

void UpgradeSystem::ApplyLevelTransition(Player& player) const {
    player.Heal(player.GetMaxHealth());
    
    SpellHand& spell_hand = player.GetSpellHand();
    size_t current_size = spell_hand.GetSize();
    
    if (current_size > 0) {
        size_t spells_to_remove = (current_size + 1) / 2;
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::vector<size_t> indices;
        for (size_t i = 0; i < current_size; ++i) {
            indices.push_back(i);
        }
        
        std::shuffle(indices.begin(), indices.end(), gen);
        
        std::vector<size_t> to_remove(indices.begin(), indices.begin() + spells_to_remove);
        std::sort(to_remove.rbegin(), to_remove.rend());
        
        for (size_t idx : to_remove) {
            spell_hand.RemoveSpell(idx);
        }
    }
}

Upgrade UpgradeSystem::CreateHealthUpgrade(int level) const {
    Upgrade upgrade;
    upgrade.type = UpgradeType::kIncreaseMaxHealth;
    upgrade.name = "Увеличить максимальное здоровье";
    upgrade.value = 20 + level * 5;
    upgrade.description = "+" + std::to_string(upgrade.value) + " к максимальному здоровью";
    return upgrade;
}

Upgrade UpgradeSystem::CreateMeleeDamageUpgrade(int level) const {
    Upgrade upgrade;
    upgrade.type = UpgradeType::kIncreaseMeleeDamage;
    upgrade.name = "Увеличить урон ближнего боя";
    upgrade.value = 5 + level * 2;
    upgrade.description = "+" + std::to_string(upgrade.value) + " к урону ближнего боя";
    return upgrade;
}

Upgrade UpgradeSystem::CreateRangedDamageUpgrade(int level) const {
    Upgrade upgrade;
    upgrade.type = UpgradeType::kIncreaseRangedDamage;
    upgrade.name = "Увеличить урон дальнего боя";
    upgrade.value = 5 + level * 2;
    upgrade.description = "+" + std::to_string(upgrade.value) + " к урону дальнего боя";
    return upgrade;
}

}

