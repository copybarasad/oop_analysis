#include "CastSpellCommand.h"
#include "Game.h"

/**
 * @file CastSpellCommand.cpp
 * @brief Реализация команды использования заклинания
 */

/**
 * @brief Выполнить использование заклинания
 * @param game Указатель на игру
 * @return true если заклинание успешно использовано
 * 
 * Использует Game::usePlayerSpell(), который:
 * 1. Применяет заклинание
 * 2. Удаляет его из руки
 * 3. Обрабатывает ходы врагов/союзников
 */
bool CastSpellCommand::execute(Game* game) {
    if (!game) {
        return false;
    }
    
    // Используем заклинание через игровую систему
    return game->usePlayerSpell(spellIndex);
}

/**
 * @brief Получить описание команды
 * @return Строка вида "Cast Spell #0"
 */
std::string CastSpellCommand::getDescription() const {
    return "Cast Spell #" + std::to_string(spellIndex);
}


