#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Game;

struct CommandResult {
    bool success;
    std::string message;
};

class Command {
 public:
  virtual ~Command() = default;

  virtual CommandResult execute(Game& game) = 0;

  virtual bool EndsTurn() const { return false; }
};

#endif  // COMMAND_H
