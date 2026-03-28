#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <string>

/**
 * @file LevelManager.h
 * @brief Управление уровнями и их параметрами
 */

/**
 * @struct LevelConfig
 * @brief Конфигурация уровня
 */
struct LevelConfig {
    int levelNumber;         ///< Номер уровня
    int fieldWidth;          ///< Ширина поля
    int fieldHeight;         ///< Высота поля
    int enemyCount;          ///< Количество врагов
    int enemyHealthBonus;    ///< Бонус к здоровью врагов
    int enemyDamageBonus;    ///< Бонус к урону врагов
    int towerCount;          ///< Количество башен
    int blockedCellsCount;   ///< Количество стен
    
    /**
     * @brief Конструктор уровня
     * @param level Номер уровня
     */
    LevelConfig(int level) : levelNumber(level) {
        fieldWidth = 15 + (level - 1) * 5;          ///< Поле растёт на 5 клеток
        fieldHeight = 15 + (level - 1) * 5;
        enemyCount = 3 + level * 2;                  ///< +2 врага за уровень
        enemyHealthBonus = (level - 1) * 20;         ///< +20 HP за уровень
        enemyDamageBonus = (level - 1) * 5;          ///< +5 урона за уровень
        towerCount = level;                          ///< Башен = номер уровня
        blockedCellsCount = 20 + level * 5;          ///< Больше стен
    }
    
    /**
     * @brief Получить описание уровня
     * @return Строка с описанием
     */
    std::string getDescription() const {
        return "Level " + std::to_string(levelNumber) + 
               " | Field: " + std::to_string(fieldWidth) + "x" + std::to_string(fieldHeight) +
               " | Enemies: " + std::to_string(enemyCount) +
               " | Towers: " + std::to_string(towerCount);
    }
};

/**
 * @class LevelManager
 * @brief Менеджер уровней
 */
class LevelManager {
private:
    int currentLevel;  ///< Текущий уровень
    
public:
    /**
     * @brief Конструктор
     */
    LevelManager() : currentLevel(1) {}
    
    /**
     * @brief Получить текущий уровень
     * @return Номер уровня
     */
    int getCurrentLevel() const {
        return currentLevel;
    }
    
    /**
     * @brief Перейти на следующий уровень
     */
    void nextLevel() {
        currentLevel++;
    }
    
    /**
     * @brief Установить уровень
     * @param level Номер уровня
     */
    void setCurrentLevel(int level) {
        if (level >= 1) {
            currentLevel = level;
        }
    }
    
    /**
     * @brief Получить конфигурацию текущего уровня
     * @return Конфигурация уровня
     */
    LevelConfig getCurrentLevelConfig() const {
        return LevelConfig(currentLevel);
    }
    
    /**
     * @brief Получить конфигурацию конкретного уровня
     * @param level Номер уровня
     * @return Конфигурация уровня
     */
    LevelConfig getLevel(int level) const {
        return LevelConfig(level);
    }
};

#endif // LEVELMANAGER_H


