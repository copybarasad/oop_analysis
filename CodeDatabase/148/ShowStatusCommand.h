#ifndef SHOWSTATUSCOMMAND_H
#define SHOWSTATUSCOMMAND_H

#include "Command.h"

/**
 * @file ShowStatusCommand.h
 * @brief Команда отображения статуса игрока
 */

/**
 * @class ShowStatusCommand
 * @brief Команда для отображения информации об игроке
 * 
 * НЕ поддерживает undo (это команда просмотра, не действие).
 */
class ShowStatusCommand : public Command {
public:
    ShowStatusCommand() = default;
    
    /**
     * @brief Выполнить отображение статуса
     * Использует GameRenderer для вывода
     */
    bool execute(Game* game) override;
    
    bool isUndoable() const override { return false; }
    
    std::string getDescription() const override;
};

#endif // SHOWSTATUSCOMMAND_H


