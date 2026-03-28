#ifndef SWITCHMODECOMMAND_H
#define SWITCHMODECOMMAND_H

#include "Command.h"
#include "GameTypes.h"

/**
 * @file SwitchModeCommand.h
 * @brief Команда переключения режима атаки игрока
 */

/**
 * @class SwitchModeCommand
 * @brief Команда для переключения между MELEE и RANGED режимами
 * 
 * Поддерживает undo (можно вернуть предыдущий режим).
 */
class SwitchModeCommand : public Command {
private:
    AttackMode previousMode;  ///< Предыдущий режим атаки (для undo)
    bool modeChanged;  ///< Был ли изменён режим
    
public:
    /**
     * @brief Конструктор
     */
    SwitchModeCommand() 
        : previousMode(AttackMode::MELEE), 
          modeChanged(false) {}
    
    /**
     * @brief Выполнить переключение режима
     */
    bool execute(Game* game) override;
    
    /**
     * @brief Отменить переключение (вернуть предыдущий режим)
     */
    bool undo(Game* game) override;
    
    /**
     * @brief Команда поддерживает undo
     */
    bool isUndoable() const override { 
        return modeChanged; 
    }
    
    /**
     * @brief Получить описание команды
     */
    std::string getDescription() const override;
};

#endif // SWITCHMODECOMMAND_H


