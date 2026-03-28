#include "SwitchModeCommand.h"
#include "Game.h"
#include "Player.h"

/**
 * @file SwitchModeCommand.cpp
 * @brief Реализация команды переключения режима атаки
 */

/**
 * @brief Выполнить переключение режима атаки
 * @param game Указатель на игру
 * @return true если режим успешно переключён
 * 
 * Переключает между MELEE (ближний бой, 40 урона) и 
 * RANGED (дальний бой, 20 урона, дальность 3).
 */
bool SwitchModeCommand::execute(Game* game) {
    if (!game || !game->getPlayer()) {
        return false;
    }
    
    Player* player = game->getPlayer();
    
    // Сохраняем текущий режим для undo
    previousMode = player->getAttackMode();
    
    // Переключаем режим
    game->switchPlayerAttackMode();
    
    // Проверяем изменился ли режим
    AttackMode newMode = player->getAttackMode();
    modeChanged = (newMode != previousMode);
    
    return modeChanged;
}

/**
 * @brief Отменить переключение режима
 * @param game Указатель на игру
 * @return true если отмена выполнена успешно
 */
bool SwitchModeCommand::undo(Game* game) {
    if (!modeChanged || !game || !game->getPlayer()) {
        return false;
    }
    
    Player* player = game->getPlayer();
    
    // Возвращаем предыдущий режим
    player->setAttackMode(previousMode);
    
    return true;
}

/**
 * @brief Получить описание команды
 * @return Строка "Switch Attack Mode"
 */
std::string SwitchModeCommand::getDescription() const {
    return "Switch Attack Mode";
}


