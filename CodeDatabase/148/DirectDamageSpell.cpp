#include "DirectDamageSpell.h"
#include "SpellHand.h"
#include "Game.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <cmath>

/**
 * @file DirectDamageSpell.cpp
 * @brief Реализация заклинания прямого урона
 */

/**
 * @brief Конструктор заклинания прямого урона
 * @param baseDamage Базовый урон (по умолчанию 50)
 * @param range Радиус действия (по умолчанию 3)
 * 
 * Инициализирует заклинание с указанными параметрами.
 */
DirectDamageSpell::DirectDamageSpell(int baseDamage, int range)
    : Spell("Direct Damage", "Deals " + std::to_string(baseDamage) + " damage to target", baseDamage),  // ← добавлен baseDamage
      damage(baseDamage), baseRadius(range) {
}


/**
 * @brief Применить заклинание (нанести урон ближайшим целям)
 * @param game Указатель на игру
 * @return true если заклинание успешно применено
 * 
 * Процесс:
 * 1. Получает позицию игрока
 * 2. Применяет усиления если активны
 * 3. Ищет всех врагов в радиусе
 * 4. Наносит урон найденным целям
 */
bool DirectDamageSpell::cast(Game* game) {
    if (!game) {
        return false;
    }

    Player* player = game->getPlayer();
    auto [px, py] = player->getPosition();
    
    // Применяем усиление если есть
    SpellHand* hand = game->getPlayerSpellHand();
    int effectiveRange = baseRadius;
    if (hand && hand->getBoostStacks() > 0) {
        effectiveRange += hand->getBoostStacks();
    }

    int hitCount = 0;
    
    // Наносим урон врагам в радиусе
    for (auto& enemy : game->getEnemies()) {
        if (!enemy->isAlive()) continue;
        
        auto [ex, ey] = enemy->getPosition();
        int dx = std::abs(px - ex);
        int dy = std::abs(py - ey);
        int distance = std::max(dx, dy);
        
        if (distance <= effectiveRange) {
            enemy->takeDamage(damage);
            hitCount++;
        }
    }

    // Наносим урон башням в радиусе
    for (auto& tower : game->getTowers()) {
        if (!tower->isAlive()) continue;
        
        auto [tx, ty] = tower->getPosition();
        int dx = std::abs(px - tx);
        int dy = std::abs(py - ty);
        int distance = std::max(dx, dy);
        
        if (distance <= effectiveRange) {
            tower->takeDamage(damage);
            hitCount++;
        }
    }

    return hitCount > 0;
}



