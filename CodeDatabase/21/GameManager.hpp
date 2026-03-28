#pragma once
#include <memory>
#include "Config.hpp"
#include "commands/Command.hpp"
#include "commands/ControlCommand.hpp"
#include "commands/PlayerActionCommand.hpp"

template <class Input, class Game>
class GameManager {
private:
    Game& gameController;
    Input& inputController;
public:
    GameManager(Input& inputController, Game& gameController) :
    inputController(inputController), gameController(gameController) {};
    ~GameManager() = default;
    void recieveAndExecuteCommand(GameState& currentState, GameState& prevState) {
        std::unique_ptr<Command> command = inputController.getCommand();
        if (command) {
            command->execute(gameController, currentState, prevState);
        }
    }
};