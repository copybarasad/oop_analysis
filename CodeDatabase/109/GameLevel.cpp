#include "GameLevel.h"
#include <algorithm>

GameLevel::GameLevel(int level) : levelNumber(level) {
    // Базовые параметры
    int baseSize = 15;
    mapWidth = std::min(baseSize + (level * 2), 30);
    mapHeight = mapWidth;
    
    enemyCount = 3 + level;
    towerCount = (level > 1) ? (level - 1) : 0;
    
    // Увеличение характеристик врагов на 10% за уровень
    float multiplier = 1.0f + (level * 0.1f);
    enemyHealth = static_cast<int>(30 * multiplier);
    enemyDamage = static_cast<int>(5 * multiplier);
    towerHealth = static_cast<int>(50 * multiplier);
    towerDamage = static_cast<int>(8 * multiplier);
    
    // Увеличение стоимости заклинаний на 5% за уровень
    spellCostMultiplier = 1.0f + (level * 0.05f);
}