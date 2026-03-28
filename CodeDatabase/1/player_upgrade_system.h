#pragma once
#include "vampire_hunter.h"
#include <vector>
#include <string>

class PlayerUpgradeSystem {
public:
    struct UpgradeOption {
        int id;
        std::string name;
        std::string description;
    };
    enum class UpgradeType {
        HEALTH,           // +25 HP
        SPELL_POWER,      // +5 к урону заклинаний
        ATTACK_DAMAGE,    // +3 к урону атаки  
        AMMO              // +2 пули, +2 колья
    };

    PlayerUpgradeSystem();
    
    // Получить список доступных улучшений
    const std::vector<UpgradeOption>& getAvailableUpgrades() const;
    
    // Применить выбранное улучшение
    void applyUpgrade(UpgradeType type, VampireHunter& hunter);
    
    // Получить тип улучшения по ID
    UpgradeType getUpgradeTypeById(int id) const;

private:
    std::vector<UpgradeOption> availableUpgrades_;
};