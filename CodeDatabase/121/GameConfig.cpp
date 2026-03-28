#include "GameConfig.h"

// Реализация игровой конфигурации
// Инициализирует все настройки баланса игры.
// Здесь можно легко изменить любые числовые значения.

GameConfig::GameConfig() {
    // Инициализация конфигурации игрока
    playerConfig = {
        .startHealth = 100,
        .baseDamage = 5,
        .closeCombatBonus = 5,
        .closeCombatRange = 1.5,
        .longRangeCombatRange = 5.0,
        .startScore = 0,
        .maxSpellSlots = 3
    };
    
    // Инициализация конфигурации врага
    enemyConfig = {
        .health = 25,
        .damage = 5,
        .reward = 10,
        .attackRange = 1.5
    };
    
    // Инициализация конфигурации здания
    buildingConfig = {
        .health = 50,
        .reward = 25,
        .spawnCooldown = 8
    };
    
    // Инициализация конфигурации башни
    towerConfig = {
        .health = 60,
        .reward = 35,
        .attackRange = 5,
        .spellDamage = 8,
        .spellCooldown = 3
    };
    
    // Инициализация конфигураций уровней
    
    // Лесной уровень (легкий)
    levelConfigs[LevelType::FOREST] = {
        .width = 15,
        .height = 15,
        .enemyCount = 2,
        .buildingCount = 0,
        .towerCount = 0,
        .enemyDensity = 0.1
    };
    
    // Замковый уровень (средний)
    levelConfigs[LevelType::CASTLE] = {
        .width = 17,
        .height = 17,
        .enemyCount = 3,
        .buildingCount = 1,
        .towerCount = 0,
        .enemyDensity = 0.15
    };
    
    // Подземный уровень (сложный)
    levelConfigs[LevelType::DUNGEON] = {
        .width = 19,
        .height = 19,
        .enemyCount = 4,
        .buildingCount = 2,
        .towerCount = 1,
        .enemyDensity = 0.2
    };
    
    // Инициализация конфигураций заклинаний
    
    // Магический выстрел
    spellConfigs[SpellType::DIRECT_DAMAGE] = {
        .name = "Магический выстрел",
        .description = "Наносит 15 урона одной цели в радиусе 5 клеток",
        .damage = 15,
        .range = 5,
        .cost = 50
    };
    
    // Огненный шар
    spellConfigs[SpellType::AREA_DAMAGE] = {
        .name = "Огненный шар",
        .description = "Наносит 10 урона по области 2x2 клетки",
        .damage = 10,
        .range = 4,
        .cost = 75
    };
    
    // Ловушка
    spellConfigs[SpellType::TRAP] = {
        .name = "Ловушка",
        .description = "Размещает ловушку, которая наносит 15 урона при активации",
        .damage = 15,
        .range = 3,
        .cost = 60
    };
}

// Получить конфигурацию уровня по типу
const LevelConfig& GameConfig::getLevelConfig(LevelType type) const {
    auto it = levelConfigs.find(type);
    if (it != levelConfigs.end()) {
        return it->second;
    }
    // Возвращаем конфигурацию по умолчанию если тип не найден
    static LevelConfig defaultConfig = {
        .width = 15,
        .height = 15,
        .enemyCount = 2,
        .buildingCount = 0,
        .towerCount = 0,
        .enemyDensity = 0.1
    };
    return defaultConfig;
}

// Получить конфигурацию заклинания по типу
const SpellConfig& GameConfig::getSpellConfig(SpellType type) const {
    auto it = spellConfigs.find(type);
    if (it != spellConfigs.end()) {
        return it->second;
    }
    // Возвращаем конфигурацию по умолчанию если тип не найден
    static SpellConfig defaultConfig = {
        .name = "Неизвестное заклинание",
        .description = "Описание отсутствует",
        .damage = 0,
        .range = 0,
        .cost = 0
    };
    return defaultConfig;
}