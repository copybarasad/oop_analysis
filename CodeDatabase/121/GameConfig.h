#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include "LevelType.h"
#include "SpellType.h"
#include "BattleMode.h"
#include "SpellType.h"
#include <unordered_map>
#include <string>


// Централизованное хранение всех игровых настроек и баланса
// Содержит структуры с настройками для всех игровых объектов.
// Позволяет легко изменять баланс игры без изменения кода.


// Структура с настройками игрока
struct PlayerConfig {
    int startHealth = 100;           // Начальное здоровье игрока
    int baseDamage = 5;              // Базовый урон игрока
    int closeCombatBonus = 5;        // Бонус к урону в ближнем бою
    double closeCombatRange = 1.5;   // Радиус атаки в ближнем бою
    double longRangeCombatRange = 5.0; // Радиус атаки в дальнем бою
    int startScore = 0;              // Начальное количество очков
    int maxSpellSlots = 3;           // Максимальное количество слотов для заклинаний
};

// Структура с настройками врага
struct EnemyConfig {
    int health = 25;                 // Здоровье врага
    int damage = 5;                  // Урон врага
    int reward = 10;                 // Очки за убийство врага
    double attackRange = 1.5;        // Радиус атаки врага
};

// Структура с настройками здания
struct BuildingConfig {
    int health = 50;                 // Здоровье здания
    int reward = 25;                 // Очки за разрушение здания
    int spawnCooldown = 8;           // Время между спаунами врагов
};

// Структура с настройками башни
struct TowerConfig {
    int health = 60;                 // Здоровье башни
    int reward = 35;                 // Очки за разрушение башни
    int attackRange = 5;             // Радиус атаки башни
    int spellDamage = 8;             // Урон заклинания башни
    int spellCooldown = 3;           // Время перезарядки заклинания
};

// Структура с настройками уровня
struct LevelConfig {
    int width;                       // Ширина уровня в клетках
    int height;                      // Высота уровня в клетках
    int enemyCount;                  // Количество врагов на уровне
    int buildingCount;               // Количество зданий на уровне
    int towerCount;                  // Количество башен на уровне
    double enemyDensity;             // Плотность врагов (0.0 - 1.0)
};

// Структура с настройками заклинания
struct SpellConfig {
    std::string name;                // Название заклинания
    std::string description;         // Описание заклинания
    int damage;                      // Урон заклинания
    int range;                       // Радиус действия заклинания
    int cost;                        // Стоимость в магазине
};

// Основной класс конфигурации игры
class GameConfig {
private:
    // Конфигурации для разных типов уровней
    std::unordered_map<LevelType, LevelConfig> levelConfigs;
    
    // Конфигурации для разных типов заклинаний
    std::unordered_map<SpellType, SpellConfig> spellConfigs;
    
    // Конфигурации игровых объектов
    PlayerConfig playerConfig;
    EnemyConfig enemyConfig;
    BuildingConfig buildingConfig;
    TowerConfig towerConfig;

public:
    // Конструктор - инициализирует все настройки
    GameConfig();
    
    // Методы для получения конфигураций
    
    // Получить конфигурацию уровня по типу
    const LevelConfig& getLevelConfig(LevelType type) const;
    
    // Получить конфигурацию заклинания по типу
    const SpellConfig& getSpellConfig(SpellType type) const;
    
    // Получить конфигурацию игрока
    const PlayerConfig& getPlayerConfig() const { return playerConfig; }
    
    // Получить конфигурацию врага
    const EnemyConfig& getEnemyConfig() const { return enemyConfig; }
    
    // Получить конфигурацию здания
    const BuildingConfig& getBuildingConfig() const { return buildingConfig; }
    
    // Получить конфигурацию башни
    const TowerConfig& getTowerConfig() const { return towerConfig; }
};

#endif