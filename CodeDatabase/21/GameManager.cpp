#include "GameManager.hpp"


template <class Input, class Game>
void GameManager<Input, Game>::recieveAndExecuteCommand(GameState& currentState, GameState& prevState) {
    std::unique_ptr<Command> command = inputController->getCommand();
    command->execute(gameController, currentState, prevState);
}