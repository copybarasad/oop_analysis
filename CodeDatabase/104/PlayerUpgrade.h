#ifndef PLAYERUPGRADE_H
#define PLAYERUPGRADE_H

#include "Player.h"
#include <vector>

// Система улучшений игрока между уровнями
class PlayerUpgrade {
public:
    // Типы доступных улучшений
    enum class UpgradeType {
        HEALTH_BOOST,    // Увеличение здоровья
        SPELL_POWER,     // Увеличение силы заклинаний
        SPELL_RANGE      // Увеличение радиуса заклинаний
    };

    PlayerUpgrade();

    // Основные методы улучшений
    void applyUpgrade(Player& player, UpgradeType type);
    void showUpgradeMenu(Player& player);
    bool isUpgradeAvailable(UpgradeType type) const;

private:
    std::vector<UpgradeType> takenUpgrades_;

    // Вспомогательные методы
    void increaseAllSpellDamage(Player& player, float multiplier);
    void increaseAllSpellRange(Player& player, int amount);
    void markUpgradeTaken(UpgradeType type);
};

#endif