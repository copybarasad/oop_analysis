#ifndef COMMAND_BASED_GAME_CONTROLLER_H
#define COMMAND_BASED_GAME_CONTROLLER_H

#include "game.h"
#include "console_input_reader.h"
#include "command.h"
#include <memory>

class CommandBasedGameController {
private:
    Game& game;
    std::unique_ptr<ConsoleInputReader> inputReader;

public:
    explicit CommandBasedGameController(Game& g, const std::string& configFile = "controls.cfg")
        : game(g), inputReader(std::make_unique<ConsoleInputReader>(configFile)) {}

    std::unique_ptr<Command> getNextCommand() {
        return inputReader->getNextCommand();
    }

    void executeCommand(const std::unique_ptr<Command>& command) {
        if (command) {
            command->execute(game);
        }
    }

    Game& getGame() { return game; }
    const Game& getGame() const { return game; }

    ConsoleInputReader& getInputReader() { return *inputReader; }
};

#endif