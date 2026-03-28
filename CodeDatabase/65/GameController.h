#pragma once

#include <memory>
#include "commands/ICommand.h"

template<typename TInputReader>
class GameController {
private:
    TInputReader inputReader;
    Game* game;

public:
    explicit GameController(Game* gameInstance) : game(gameInstance) {}

    void loadConfiguration(const std::string& configFile) {
        inputReader.loadKeybindings(configFile);
    }

    std::unique_ptr<ICommand> getNextCommand() {
        return inputReader.getCommand();
    }

    char getChar() {
        return inputReader.getChar();
    }

    void executeCommand(std::unique_ptr<ICommand> command) {
        if (command && game) {
            command->execute(*game);
        }
    }
};
