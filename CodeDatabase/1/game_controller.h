#pragma once

#include "game.h"
#include "command_input.h"
#include "command.h"
#include <iostream>

template<typename InputHandlerType>
class GameController {
private:
    Game& game_;
    InputHandlerType input_handler_;

public:
    explicit GameController(Game& game) 
        : game_(game), 
          input_handler_(game_.getUIEventManager().getEventSystem())
    {}
    
    bool processInput() {
        Command* command = input_handler_.getNextCommand();
        if (!command) {
            return false;
        }
        
        try {
            EventSystem* event_system = game_.getUIEventManager().getEventSystem();
            CommandResult result = command->execute(game_, event_system);
            
            if (!result.message.empty()) {
                std::cout << result.message << "\n";
            }
            
            return result.turnConsumed;
            
        } catch (const std::exception& e) {
            std::cout << "Ошибка выполнения команды: " << e.what() << "\n";
            return false;
        }
    }
    
    bool processPlayerTurn() {
        bool turn_completed = false;
        
        while (!turn_completed && game_.isGameRunning()) {
            if (!turn_completed) {
                showTurnPrompt();
            }
            
            turn_completed = processInput();
        }
        
        return turn_completed;
    }

private:
    void showTurnPrompt() {
        std::cout << "\n🎮 ВАШ ХОД | ";
        std::cout << "Здоровье: " << game_.getHunter().getHealth() << "/" 
                  << game_.getHunter().getMaxHealth() << " | ";
        std::cout << "Дракула: " << game_.getDracula().getHealth() << " HP";
        
        if (game_.getDracula().isInvulnerable()) {
            std::cout << " 💫";
        }
        std::cout << "\nВведите команду (H для помощи): ";
    }
};