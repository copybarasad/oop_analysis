#include "AreaDamageSpell.h"
#include "SpellHand.h"
#include "Game.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <cmath>

/**
 * @file AreaDamageSpell.cpp
 * @brief Реализация заклинания урона по площади
 */

/**
 * @brief Конструктор заклинания урона по площади
 * @param baseDamage Базовый урон (по умолчанию 25)
 * @param size Размер области воздействия (по умолчанию 2x2)
 */
AreaDamageSpell::AreaDamageSpell(int baseDamage, int size)
    : Spell("Area Damage", "Deals " + std::to_string(baseDamage) + " damage in " + std::to_string(size) + "x" + std::to_string(size) + " area", baseDamage),
      damage(baseDamage), areaSize(size) {
}

/**
 * @brief Применить заклинание БЕЗ координат (не поддерживается)
 * @return false всегда
 */
bool AreaDamageSpell::cast(Game* game) {
    // Area Damage всегда требует координаты
    return false;
}

/**
 * @brief Применить заклинание С координатами (нанести урон в области)
 * @param game Указатель на игру
 * @param targetX X-координата центра области
 * @param targetY Y-координата центра области
 * @return true если заклинание успешно применено
 */
bool AreaDamageSpell::cast(Game* game, int targetX, int targetY) {
    if (!game) {
        return false;
    }

    const GameField* field = game->getField();
    
    // Применяем усиление если есть
    int effectiveSize = areaSize;
    SpellHand* hand = game->getSpellHand();
    if (hand && hand->getBoostStacks() > 0) {
        effectiveSize += hand->getBoostStacks();
    }

    int hitCount = 0;
    
    // Наносим урон врагам в области targetX,targetY с размером effectiveSize x effectiveSize
    for (auto& enemy : game->getEnemies()) {
        if (!enemy->isAlive()) continue;
        
        auto [ex, ey] = enemy->getPosition();
        if (std::abs(ex - targetX) < effectiveSize && 
            std::abs(ey - targetY) < effectiveSize) {
            enemy->takeDamage(damage);
            hitCount++;
        }
    }

    // Наносим урон башням в области
    for (auto& tower : game->getTowers()) {
        if (!tower->isAlive()) continue;
        
        auto [tx, ty] = tower->getPosition();
        if (std::abs(tx - targetX) < effectiveSize && 
            std::abs(ty - targetY) < effectiveSize) {
            tower->takeDamage(damage);
            hitCount++;
        }
    }

    // Заклинание используется всегда, даже если никого не задело
    return true;
}


