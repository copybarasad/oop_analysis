#ifndef COMMAND_INTERFACE_H
#define COMMAND_INTERFACE_H

#include "game_controller.h"
#include "command.h" 
#include <string>
#include <memory>

// Интерфейс для всех команд игры
class ICommand {
public:
    virtual ~ICommand() = default;
    
    // Выполнить команду
    virtual bool execute(GameController& game) = 0;
    
    // Получить описание команды
    virtual std::string getDescription() const = 0;
    
    // Проверить валидность команды
    virtual bool isValid() const = 0;
    
    // Получить сообщение об ошибке
    virtual std::string getErrorMessage() const { return ""; }
    
    // Получить тип команды
    virtual CommandType getCommandType() const = 0;
    
    // Получить дополнительные данные команды (например, индекс сохранения)
    virtual int getCommandData() const { return -1; }
};

// Указатель на команду
using CommandPtr = std::unique_ptr<ICommand>;

#endif