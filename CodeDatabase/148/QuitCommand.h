#ifndef QUITCOMMAND_H
#define QUITCOMMAND_H

#include "Command.h"

/**
 * @file QuitCommand.h
 * @brief Команда выхода из игры
 */

/**
 * @class QuitCommand
 * @brief Команда для завершения игры
 * 
 * НЕ поддерживает undo.
 */
class QuitCommand : public Command {
public:
    /**
     * @brief Конструктор
     */
    QuitCommand() = default;
    
    /**
     * @brief Выполнить выход из игры
     */
    bool execute(Game* game) override;
    
    /**
     * @brief Выход не поддерживает undo
     */
    bool isUndoable() const override { 
        return false; 
    }
    
    /**
     * @brief Получить описание команды
     */
    std::string getDescription() const override;
};

#endif // QUITCOMMAND_H


