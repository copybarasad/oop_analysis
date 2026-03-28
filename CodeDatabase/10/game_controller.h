#pragma once
#include <memory>
#include "input_provider.h"
#include "game.h"

template<typename InputProviderType>
class GameController {
private:
    InputProviderType& inputProvider;
    std::string lastCommand;
    
public:
    explicit GameController(InputProviderType& provider) 
        : inputProvider(provider) {}
    
    bool processCommand(Game& game) {
        inputProvider.showPrompt("Введите команду: ");
        auto command = inputProvider.getNextCommand();
        
        if (!command) {
            return false;
        }
        
        lastCommand = command->getName();
        return command->execute(game);
    }
    
    std::string getLastCommand() const { return lastCommand; }
};
