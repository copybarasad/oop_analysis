#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Command.h"
#include <memory>


class GameManager;



template <typename TInputReader> class GameController {
private:
  TInputReader inputReader;
  GameManager *gameManager;

public:
  GameController(GameManager *manager, TInputReader reader)
      : inputReader(std::move(reader)), gameManager(manager) {}

  
  std::unique_ptr<Command> getNextCommand() {
    return inputReader.readCommand();
  }

  
  bool processCommand(Command *cmd) {
    if (!cmd || !gameManager) {
      return false;
    }
    return cmd->execute(gameManager);
  }

  
  TInputReader &getInputReader() { return inputReader; }

  const TInputReader &getInputReader() const { return inputReader; }
};

#endif 
