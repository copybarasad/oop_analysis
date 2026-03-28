#ifndef COMMAND_INPUT_INTERFACE_H
#define COMMAND_INPUT_INTERFACE_H

#include "command_interface.h"
#include "game_controller.h"
#include <string>

// Интерфейс для классов ввода команд
class ICommandInput {
public:
    virtual ~ICommandInput() = default;
    
    // Чтение команд из различных состояний игры
    virtual CommandPtr readMainMenuCommand() = 0;
    virtual CommandPtr readLoadMenuCommand(int saveFilesCount) = 0;
    virtual CommandPtr readInGameCommand(const GameController& game) = 0;
    virtual CommandPtr readInGameMenuCommand() = 0;
};

#endif