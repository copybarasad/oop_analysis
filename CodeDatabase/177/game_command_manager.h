#ifndef GAME_COMMAND_MANAGER_H
#define GAME_COMMAND_MANAGER_H

#include "command_input_interface.h"
#include "command_processor.h"
#include "command_interface.h"
#include "game_controller.h"
#include <memory>
#include <functional>
#include <type_traits>

// Шаблонный класс управления игрой
template<typename TCommandInput>
class GameCommandManager {
private:
    std::unique_ptr<TCommandInput> commandInput;
    
public:
    // Конструктор с передачей аргументов для TCommandInput
    template<typename... Args>
    explicit GameCommandManager(Args&&... args)
        : commandInput(std::make_unique<TCommandInput>(std::forward<Args>(args)...)) {
        static_assert(std::is_base_of_v<ICommandInput, TCommandInput>,
                     "TCommandInput must inherit from ICommandInput");
    }
    
    // Получить объект ввода команд
    TCommandInput& getCommandInput() { return *commandInput; }
    const TCommandInput& getCommandInput() const { return *commandInput; }
    
    // Обработать команду из главного меню
    bool processMainMenuCommand(CommandProcessor& processor) const {
        CommandPtr cmd = commandInput->readMainMenuCommand();
        return processor.processCommand(std::move(cmd));
    }
    
    // Обработать команду из меню загрузки
    bool processLoadMenuCommand(int saveFilesCount, CommandProcessor& processor,
                                std::function<void(int)> loadCallback) const {
        CommandPtr cmd = commandInput->readLoadMenuCommand(saveFilesCount);
        
        if (cmd && cmd->isValid()) {
            processor.processCommand(std::move(cmd));
            return false; // Продолжить (игра загружена)
        }
        
        return false; 
    }
    
    // Обработать команду во время игры
    bool processInGameCommand(CommandProcessor& processor) const {
        if (!processor.getGame()) {
            return false;
        }
        
        CommandPtr cmd = commandInput->readInGameCommand(*processor.getGame());
        return processor.processCommand(std::move(cmd));
    }
    
    // Обработать команду из игрового меню (пауза)
    bool processInGameMenuCommand(CommandProcessor& processor,
                                   std::function<void()> saveCallback,
                                   std::function<void()> returnToMainMenuCallback) const {
        CommandPtr cmd = commandInput->readInGameMenuCommand();
        
        if (cmd && cmd->isValid()) {
            bool continueGame = processor.processCommand(std::move(cmd));
            return continueGame;
        }
        
        return true;
    }
};

#endif