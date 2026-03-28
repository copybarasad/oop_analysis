#include "BoostSpell.h"
#include "SpellHand.h"
#include "Game.h"

/**
 * @file BoostSpell.cpp
 * @brief Реализация заклинания улучшения
 */

/**
 * @brief Конструктор заклинания улучшения
 */
BoostSpell::BoostSpell()
    : Spell("Boost", "Enhances the next spell (stacks)", 0) { 
}


/**
 * @brief Применить заклинание (добавить стак улучшения)
 * @param game Указатель на игру
 * @return true всегда (улучшение никогда не может провалиться)
 * 
 * Добавляет один стак улучшения в руку игрока.
 * Стаки накапливаются и применяются к следующему заклинанию.
 */
bool BoostSpell::cast(Game* game) {
    if (!game) {
        return false;
    }

    SpellHand* hand = game->getPlayerSpellHand();
    if (!hand) {
        return false;
    }

    hand->addBoostStack();
    
    return true;
}



