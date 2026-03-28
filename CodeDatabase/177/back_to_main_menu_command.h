#ifndef BACK_TO_MAIN_MENU_COMMAND_H
#define BACK_TO_MAIN_MENU_COMMAND_H

#include "command_interface.h"
#include "game_controller.h"
#include "command.h"

class BackToMainMenuCommand : public ICommand {
public:
    bool execute(GameController& game) override {
        (void)game;  // Игнорируем параметр
        return true; // Продолжаем игру
    }
    
    std::string getDescription() const override {
        return "Back to Main Menu";
    }
    
    bool isValid() const override {
        return true;
    }
    
    CommandType getCommandType() const override {
        return CommandType::BackToMainMenu;
    }
    
    std::string getErrorMessage() const override {
        return "";
    }
};

#endif