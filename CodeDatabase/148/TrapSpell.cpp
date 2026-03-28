#include "TrapSpell.h"
#include "SpellHand.h"
#include "Game.h"
#include "Trap.h"

/**
 * @file TrapSpell.cpp
 * @brief Реализация заклинания установки ловушки
 */

/**
 * @brief Конструктор заклинания ловушки
 * @param baseDamage Базовый урон ловушки (по умолчанию 35)
 */
TrapSpell::TrapSpell(int baseDamage)
    : Spell("Trap", "Places a trap that deals " + std::to_string(baseDamage) + " damage", baseDamage),
      trapDamage(baseDamage) {
}

/**
 * @brief Применить заклинание БЕЗ координат (не поддерживается)
 * @return false всегда
 */
bool TrapSpell::cast(Game* game) {
    // Trap всегда требует координаты
    return false;
}

/**
 * @brief Применить заклинание С координатами (установить ловушку)
 * @param game Указатель на игру
 * @param targetX X-координата ловушки
 * @param targetY Y-координата ловушки
 * @return true если заклинание успешно применено
 */
bool TrapSpell::cast(Game* game, int targetX, int targetY) {
    if (!game) {
        return false;
    }

    // Проверка валидности координат
    const GameField* field = game->getField();
    if (targetX < 0 || targetX >= field->getWidth() ||
        targetY < 0 || targetY >= field->getHeight()) {
        return false;
    }

    // Применяем усиление если есть
    int effectiveDamage = trapDamage;
    SpellHand* hand = game->getSpellHand();
    if (hand && hand->getBoostStacks() > 0) {
        effectiveDamage += hand->getBoostStacks() * 15;  // +15 урона за стак
    }

    // Создаём ловушку на указанной позиции
    auto trap = std::make_unique<Trap>(targetX, targetY, effectiveDamage);
    game->addTrap(std::move(trap));

    return true;
}


