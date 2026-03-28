#ifndef PLAYERUPGRADE_H
#define PLAYERUPGRADE_H

#include <string>
#include <vector>

/**
 * @file PlayerUpgrade.h
 * @brief Система прокачки игрока между уровнями
 */

/**
 * @enum UpgradeType
 * @brief Типы улучшений
 */
enum class UpgradeType {
    INCREASE_MAX_HEALTH,      ///< Увеличить максимальное здоровье
    INCREASE_DAMAGE,          ///< Увеличить урон
    INCREASE_SPELL_HAND_SIZE, ///< Увеличить размер руки заклинаний
    HEAL_PLAYER               ///< Восстановить здоровье
};

/**
 * @struct Upgrade
 * @brief Описание улучшения
 */
struct Upgrade {
    UpgradeType type;      ///< Тип улучшения
    std::string name;      ///< Название
    std::string description;  ///< Описание
    int value;             ///< Значение улучшения
    
    /**
     * @brief Конструктор улучшения
     */
    Upgrade(UpgradeType t, const std::string& n, const std::string& d, int v)
        : type(t), name(n), description(d), value(v) {}
};

/**
 * @class PlayerUpgradeSystem
 * @brief Система улучшений игрока
 */
class PlayerUpgradeSystem {
private:
    std::vector<Upgrade> availableUpgrades;  ///< Доступные улучшения
    
public:
    /**
     * @brief Конструктор
     */
    PlayerUpgradeSystem() {
        initializeUpgrades();
    }

    /**
     * @brief Инициализировать список улучшений
     */
    void initializeUpgrades() {
        availableUpgrades.clear();
        
        availableUpgrades.push_back(Upgrade(
            UpgradeType::INCREASE_MAX_HEALTH,
            "Increase Max Health",
            "Permanently increase maximum health by 30",
            30
        ));
        
        availableUpgrades.push_back(Upgrade(
            UpgradeType::INCREASE_DAMAGE,
            "Increase Damage",
            "Permanently increase damage by 10",
            10
        ));
        
        availableUpgrades.push_back(Upgrade(
            UpgradeType::INCREASE_SPELL_HAND_SIZE,
            "Expand Spell Hand",
            "Increase spell hand capacity by 1",
            1
        ));
        
        availableUpgrades.push_back(Upgrade(
            UpgradeType::HEAL_PLAYER,
            "Full Heal",
            "Restore all health",
            9999
        ));
    }

    /**
     * @brief Получить улучшение по индексу
     * @param index Индекс (1-based)
     * @return Улучшение или nullptr
     */
    const Upgrade* getUpgrade(int index) const {
        if (index < 1 || index > static_cast<int>(availableUpgrades.size())) {
            return nullptr;
        }
        return &availableUpgrades[index - 1];
    }

    /**
     * @brief Получить количество доступных улучшений
     * @return Количество
     */
    int getUpgradeCount() const {
        return availableUpgrades.size();
    }
    
    /**
     * @brief Получить все улучшения
     * @return Вектор улучшений
     */
    const std::vector<Upgrade>& getAllUpgrades() const {
        return availableUpgrades;
    }
};

#endif // PLAYERUPGRADE_H


