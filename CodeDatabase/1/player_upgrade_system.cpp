#include "player_upgrade_system.h"

PlayerUpgradeSystem::PlayerUpgradeSystem() {
    // Инициализируем список улучшений
    availableUpgrades_ = {
        {1, "HEALTH", "КРЕПКОЕ ЗДОРОВЬЕ"},
        {2, "SPELL_POWER", "МОЩЬ ЗАКЛИНАНИЙ"},
        {3, "ATTACK_DAMAGE", "СМЕРТЕЛЬНЫЕ УДАРЫ"},
        {4, "AMMO", "БОЕПРИПАСЫ"}
    };
}

const std::vector<PlayerUpgradeSystem::UpgradeOption>& PlayerUpgradeSystem::getAvailableUpgrades() const {
    return availableUpgrades_;
}

void PlayerUpgradeSystem::applyUpgrade(UpgradeType type, VampireHunter& hunter) {
    switch (type) {
        case UpgradeType::AMMO:
            hunter.setAmmoUpgrade(true);
            break;
            
        case UpgradeType::HEALTH:
            hunter.upgradeHealth(25);
            break;
            
        case UpgradeType::ATTACK_DAMAGE:
            hunter.upgradeAttackDamage(3);
            break;
            
        case UpgradeType::SPELL_POWER:
            hunter.upgradeSpellPower(5);
            break;
    }
}

PlayerUpgradeSystem::UpgradeType PlayerUpgradeSystem::getUpgradeTypeById(int id) const {
    switch (id) {
        case 1: return UpgradeType::HEALTH;
        case 2: return UpgradeType::SPELL_POWER;
        case 3: return UpgradeType::ATTACK_DAMAGE;
        case 4: return UpgradeType::AMMO;
      default: 
            throw std::invalid_argument("Неверный ID улучшения: " + std::to_string(id));
    }
}
