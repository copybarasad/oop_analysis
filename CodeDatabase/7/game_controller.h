#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "command.h"
#include "input_reader.h"
#include "game_vizualizator.h"
#include <memory>
#include <string>
#include <iostream>

using namespace std;

template<typename InputProvider, typename RendererType>
class GameController {
private:
    InputProvider inputProvider;
    string lastCommand;
    
public:
    explicit GameController(const InputProvider& provider) 
        : inputProvider(provider), lastCommand("") {}
    
    bool processInput(Game& game, GameVisualization<RendererType>& visualization) {
        try {
            auto command = inputProvider.readCommand();
            if (!command) {
                visualization.renderMessage("Неверная команда!");
                // !!! ЗДЕСЬ БЫЛА ОШИБКА !!!
                // Было return false (выйти из игры), стало return true (продолжить игру)
                return true; 
            }
            
            string resultMessage;
            bool success = command->execute(game, resultMessage);
            
            lastCommand = command->getType();
            
            if (!resultMessage.empty()) {
                visualization.renderMessage(resultMessage);
            }
            
            // Выходим только если команда QUIT
            if (command->getType() == "QUIT") {
                return false;
            }
            
            return success;
            
        } catch (const exception& e) {
            visualization.renderMessage("Ошибка: " + string(e.what()));
            // При ошибке тоже не выходим, а продолжаем
            return true;
        }
    }
    
    string getLastCommand() const {
        return lastCommand;
    }
    
    InputProvider& getInputProvider() { 
        return inputProvider; 
    }
    
    const InputProvider& getInputProvider() const { 
        return inputProvider; 
    }
    
    virtual ~GameController() = default;
};

#endif // GAME_CONTROLLER_H